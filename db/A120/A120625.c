/*
 * A120625  Numbers n such that the n-th Catalan number C(2n,n)/(n+1) is
 *          divisible by 3n.
 * https://oeis.org/A120625
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m such that Catalan(m) = C(2m,m)/(m+1)
 *            is divisible by 3m (equivalently, C(2m,m) divisible by 3m(m+1))
 * Range:     u8 n <= 21, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* --------------------------------------------------------------------
   We generate the sequence lazily.  For each integer k starting at 1
   we test whether k satisfies the divisibility condition.  All tests
   are carried out on 64‑bit indices (idx_t = uint64_t).  The
   resulting term values are stored as seq_t so the code works for
   arbitrary‑precision seq_t types as well.
   -------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* --------------------------------------------------------------------
   Static state for memoisation of terms and SPF sieve.
   -------------------------------------------------------------------- */

static seq_t *terms = NULL;           /* array of sequence terms      */
static size_t terms_len = 0;          /* number of terms generated    */
static size_t terms_cap = 0;          /* array capacity               */

static uint32_t *spf = NULL;          /* smallest prime factor array  */
static uint32_t spf_limit = 0;        /* current maximum index in spf */

/* --------------------------------------------------------------------
   Helper: ensure terms array has at least 'needed' capacity.
   -------------------------------------------------------------------- */
static void ensure_terms_cap(size_t needed)
{
    if (terms_cap >= needed)
        return;
    size_t newcap = terms_cap ? terms_cap * 2 : 16;
    while (newcap < needed)
        newcap *= 2;
    seq_t *newp = (seq_t *)realloc(terms, newcap * sizeof(seq_t));
    if (!newp)
        exit(EXIT_FAILURE);
    terms = newp;
    terms_cap = newcap;
}

/* --------------------------------------------------------------------
   Build SPF (smallest prime factor) table up to 'new_limit'.
   -------------------------------------------------------------------- */
static void build_spf(uint32_t new_limit)
{
    if (spf_limit >= new_limit)
        return;

    free(spf);
    spf = (uint32_t *)calloc((size_t)new_limit + 1, sizeof(uint32_t));
    if (!spf)
        exit(EXIT_FAILURE);
    spf_limit = new_limit;

    for (uint32_t i = 2; (uint64_t)i * i <= new_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            for (uint64_t j = (uint64_t)i * i; j <= new_limit; j += i)
                if (spf[j] == 0)
                    spf[j] = i;
        }
    }
    for (uint32_t i = 2; i <= new_limit; ++i)
        if (spf[i] == 0)
            spf[i] = i;
}

/* --------------------------------------------------------------------
   Sum of digits of n in base 'base'.
   -------------------------------------------------------------------- */
static uint64_t sum_digits_base(idx_t n, uint32_t base)
{
    uint64_t sum = 0;
    while (n) {
        sum += n % base;
        n /= base;
    }
    return sum;
}

/* --------------------------------------------------------------------
   Test whether integer m satisfies the divisibility condition.
   -------------------------------------------------------------------- */
static bool satisfies(idx_t m)
{
    /* factor m using SPF */
    idx_t tmp = m;
    uint32_t primes[64];
    uint32_t exps[64];
    size_t pcnt = 0;
    while (tmp > 1) {
        uint32_t p = spf[(uint32_t)tmp];
        uint32_t cnt = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++cnt;
        }
        primes[pcnt] = p;
        exps[pcnt] = cnt;
        ++pcnt;
    }

    /* check each prime factor, including the factor 3 from 3m */
    for (size_t i = 0; i < pcnt; ++i) {
        uint32_t p = primes[i];
        uint32_t required = exps[i];
        if (p == 3)
            required += 1;           /* factor 3 in the multiplier */

        uint64_t s_m   = sum_digits_base(m, p);
        uint64_t s_2m  = sum_digits_base(m * 2, p);
        uint64_t exp_binom = (2 * s_m - s_2m) / (p - 1);

        uint64_t t = m + 1;
        uint32_t exp_plus = 0;
        while (t % p == 0) {
            t /= p;
            ++exp_plus;
        }

        uint64_t val = exp_binom - exp_plus;
        if (val < required)
            return false;
    }

    /* handle the prime 3 if it is not a factor of m */
    bool has3 = false;
    for (size_t i = 0; i < pcnt; ++i)
        if (primes[i] == 3) {
            has3 = true;
            break;
        }
    if (!has3) {
        uint32_t p = 3;
        uint32_t required = 1;
        uint64_t s_m   = sum_digits_base(m, p);
        uint64_t s_2m  = sum_digits_base(m * 2, p);
        uint64_t exp_binom = (2 * s_m - s_2m) / (p - 1);

        uint64_t t = m + 1;
        uint32_t exp_plus = 0;
        while (t % p == 0) {
            t /= p;
            ++exp_plus;
        }

        uint64_t val = exp_binom - exp_plus;
        if (val < required)
            return false;
    }

    return true;
}

/* --------------------------------------------------------------------
   Generate terms until at least 'required' terms have been found.
   -------------------------------------------------------------------- */
static void generate_terms_until(size_t required)
{
    static idx_t next_candidate = 1;          /* next integer to test      */

    while (terms_len < required) {
        /* ensure SPF covers next_candidate+1 */
        if ((idx_t)(next_candidate + 1) > spf_limit) {
            uint32_t newlimit = spf_limit ? spf_limit * 2 : 100000;
            while ((uint32_t)(next_candidate + 1) > newlimit)
                newlimit *= 2;
            build_spf(newlimit);
        }

        if (satisfies(next_candidate)) {
            ensure_terms_cap(terms_len + 1);
            terms[terms_len++] = (seq_t)next_candidate;
        }
        ++next_candidate;
    }
}

/* --------------------------------------------------------------------
   Public function required by the interface.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                   /* safety for unused n=0      */
    generate_terms_until((size_t)n);
    return terms[n - 1];
}
