/*
 * A181894  Sum of factors from A050376 in Fermi-Dirac representation of n.
 * https://oeis.org/A181894
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the sum of the prime powers in the Fermi‑Dirac
 *            representation of n. For each prime factor p^e of n, the
 *            exponent e is expressed as a sum of distinct allowed Fermi‑Dirac
 *            exponents (the smallest such set is the set A050376). The value
 *            a(n) is the sum of all p^e_i where e = e_1+e_2+…+e_k. Fo...
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*  Maximum exponent needed for n up to about 10^4 (2^14 > 10000) */
#define EMAX 64

/*  Table of admissible exponents (members of A050376) and flag to
    indicate whether it has been built. */
static bool allowed[EMAX + 1] = {0};
static bool allowed_init = false;

/*  Build the table of allowed exponents – executed once.  */
static void init_allowed(void)
{
    if (allowed_init)
        return;
    allowed[1] = true;
    allowed[2] = true;
    for (int e = 3; e <= EMAX; ++e) {
        /* gather all previously allowed exponents */
        int pool[EMAX];
        int k = 0;
        for (int i = 1; i < e; ++i)
            if (allowed[i])
                pool[k++] = i;

        /* check whether e can be expressed as a sum of distinct earlier
           allowed exponents.  */
        bool decomposable = false;
        int total = 1 << k;                      /* at most 2^k subsets */
        for (int mask = 1; mask < total && !decomposable; ++mask) {
            int sum = 0;
            for (int j = 0; j < k; ++j)
                if (mask & (1 << j))
                    sum += pool[j];
            if (sum == e)
                decomposable = true;
        }
        allowed[e] = !decomposable;
    }
    allowed_init = true;
}

/*  Small integer power (base * base * … * base) – exponent is
    guaranteed to be ≤ EMAX.  */
static seq_t int_pow(seq_t base, int exp)
{
    seq_t result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

/*  Find one decomposition of e into distinct allowed exponents.
    Assumes e is NOT in the allowed set.  The found exponents are
    stored in out[0..cnt-1].  */
static void get_decomposition(int e, int *out, int *cnt)
{
    int pool[EMAX];
    int k = 0;
    for (int i = 1; i < e; ++i)
        if (allowed[i])
            pool[k++] = i;

    int total = 1 << k;
    for (int mask = 1; mask < total; ++mask) {
        int sum = 0;
        int t = 0;
        for (int j = 0; j < k; ++j) {
            if (mask & (1 << j)) {
                sum += pool[j];
                out[t++] = pool[j];
            }
        }
        if (sum == e) {
            *cnt = t;
            return;
        }
    }
    *cnt = 0;                      /* should never happen */
}

/*  Simple trial division factorisation of n <= ~10^4.
    Stores distinct primes in primes[] and their exponents in exps[].
    On exit, *cnt contains the number of distinct prime factors.  */
static void factor_small(idx_t n, uint64_t *primes, uint64_t *exps, size_t *cnt)
{
    idx_t tmp = n;
    size_t c = 0;
    for (uint64_t p = 2; (uint64_t)p * p <= tmp; ++p) {
        if (tmp % p == 0) {
            uint64_t e = 0;
            while (tmp % p == 0) {
                tmp /= p;
                ++e;
            }
            primes[c] = p;
            exps[c] = e;
            ++c;
        }
    }
    if (tmp > 1) {
        primes[c] = tmp;
        exps[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/*  Main sequence function.  */
seq_t a(idx_t n)
{
    if (n == 1)
        return 0;

    init_allowed();

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_small(n, primes, exps, &cnt);

    seq_t sum = 0;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t e = exps[i];
        if (e <= (uint64_t)EMAX && allowed[(int)e]) {
            sum += int_pow((seq_t)p, (int)e);
        } else {
            int part[EMAX];
            int part_cnt;
            get_decomposition((int)e, part, &part_cnt);
            for (int j = 0; j < part_cnt; ++j)
                sum += int_pow((seq_t)p, part[j]);
        }
    }
    return sum;
}
