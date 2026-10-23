/*
 * A142470  Triangle T(n, k) = ( (k+2)/(2*binomial(k+2, 2)^2) )*binomial(n,
 *          k)^2*binomial(n+1, k)*binomial(n+2, k), read by rows.
 * https://oeis.org/A142470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   T(n,k) = ( (k+2)/(2*binomial(k+2,2)^2) ) * binomial(n,k)^2 *
 *            binomial(n+1,k) * binomial(n+2,k), read by rows. a(n) is the
 *            n‑th element of this triangle
 * Range:     u8 n <= 11, u16 n <= 30, u32 n <= 82, u64 n <= 239,
 *            u128 n <= 756, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

 /* ---- local constants ----------------------------------------------------- */
#define MAX_VAL 12000            /* enough for n+3 when n<=10000 */
#define MAX_PRIMES 2000          /* number of primes < MAX_VAL   */

 /* ---- static data ---------------------------------------------------------- */
static int      primes[MAX_PRIMES];
static int      prime_index[MAX_VAL];
static int      prime_count = 0;
static void init_primes(void);

 /* ---- prime tables --------------------------------------------------------- */
static void init_primes(void)
{
    static bool inited = false;
    if (inited) return;

    /* sieve up to MAX_VAL */
    static bool composite[MAX_VAL];
    for (int i = 2; i < MAX_VAL; ++i) {
        if (!composite[i]) {
            if (prime_count < MAX_PRIMES)
                primes[prime_count++] = i;
            if ((int64_t)i * i < MAX_VAL) {
                for (int j = i * i; j < MAX_VAL; j += i)
                    composite[j] = true;
            }
        }
    }

    /* build index lookup */
    for (int i = 0; i < MAX_VAL; ++i) prime_index[i] = -1;
    for (int i = 0; i < prime_count; ++i)
        prime_index[primes[i]] = i;

    inited = true;
}

/* ---- factorisation helpers ----------------------------------------------- */
static void add_factor(uint64_t val, int sign, int *exp)
{
    if (val < 2) return;
    for (int pi = 0; pi < prime_count && (uint64_t)primes[pi] * (uint64_t)primes[pi] <= val; ++pi) {
        int p = primes[pi];
        int cnt = 0;
        while (val % p == 0) { val /= p; ++cnt; }
        if (cnt) exp[pi] += sign * cnt;
    }
    if (val > 1) {                  /* remaining prime factor */
        int idx = prime_index[(int)val];
        if (idx >= 0) exp[idx] += sign;
    }
}

static void add_binom(idx_t n, idx_t k, int sign, int *exp)
{
    for (idx_t i = 0; i < k; ++i) {
        add_factor((uint64_t)(n - i),        sign, exp);   /* numerator term */
        add_factor((uint64_t)(i + 1),      -sign, exp);   /* denominator term */
    }
}

/* ---- compute a single entry T(n,k) --------------------------------------- */
static seq_t compute_T(idx_t n, idx_t k)
{
    init_primes();                    /* ensure prime tables are ready */

    int exp[MAX_PRIMES] = {0};        /* exponents of all primes */

    /* numerator terms */
    add_binom(n,     k, +1, exp);      /* C(n,k)          */
    add_binom(n,     k, +1, exp);      /* C(n,k)          */
    add_binom(n + 1, k, +1, exp);      /* C(n+1,k)        */
    add_binom(n + 2, k, +1, exp);      /* C(n+2,k)        */
    add_factor((uint64_t)(k + 2), +1, exp);            /* (k+2)         */
    /* denominator terms */
    add_factor(2U, -1, exp);                               /* the 2          */
    add_binom(k + 2, 2, -1, exp);      /* C(k+2,2)^2 part */
    add_binom(k + 2, 2, -1, exp);      /* C(k+2,2)^2 part */

    /* construct the result from prime exponents */
    seq_t res = 1;
    for (int i = 0; i < prime_count; ++i) {
        int e = exp[i];
        if (e > 0) {
            seq_t p = primes[i];
            for (int j = 0; j < e; ++j)
                res *= p;
        }
    }
    return res;
}

/* ---- main function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    idx_t pos = n;            /* position within the row-major listing */
    idx_t row = 0;
    while (1) {
        idx_t len = row + 1;   /* length of this row            */
        if (pos < len) {
            /* column k in this row is 'pos'                */
            return compute_T(row, pos);
        }
        pos -= len;            /* skip entire row                  */
        ++row;
    }
}
