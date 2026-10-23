/*
 * A172089  Triangle T(n,m) = n!/(m!!*(n-m)!!) read by rows, where (.)!! =
 *          A006882(.) are double factorials.
 * https://oeis.org/A172089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 80, u32 n <= 177, u64 n <= 473,
 *            u128 n <= 1295, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A172089
 *
 *  Triangle T(n,m) = n! / (m!! * (n-m)!!), read row‑wise.
 *  a(n) is the element at linear index n (offset 0).
 *
 *  Implementation uses prime factorization: exponent of a prime p in
 *      T(r,c) = exp_p(r!) - exp_p(c!!) - exp_p((r-c)!!)
 *
 *  Exponent of p in n! (Legendre):
 *      exp_p(n!) = floor(n/p) + floor(n/p²) + …
 *
 *  Exponent of p in n!! :
 *  • n = 2k   (even):  n!! = 2^k * k!          → exp = k*δ_{p,2} + exp_p(k!)
 *  • n = 2k‑1 (odd) : (2k‑1)!! = (2k)! / (2^k * k!)
 *                       → exp = exp_p((2k)!) - k*δ_{p,2} - exp_p(k!)
 *
 *  The code finds the row r and column c from the linear index n,
 *  then multiplies the prime powers to obtain the result.
 */

/* --------------------------------------------------------------------------- */
#include <stddef.h>   /* size_t */
#include <stdbool.h>  /* bool */

/* --------------------------------------------------------------------------- */
/* prime list – sufficient for r up to ~150 (n ≤ 10000)                          */
#define PRIME_MAX 200
static int primes[PRIME_MAX];
static size_t prime_cnt = 0;
static bool primes_built = false;

static void build_primes(void)
{
    bool composite[PRIME_MAX] = { false };
    for (int i = 2; i < PRIME_MAX; ++i) {
        if (!composite[i]) {
            primes[prime_cnt++] = i;
            for (int j = i * i; j < PRIME_MAX; j += i)
                composite[j] = true;
        }
    }
}

/* --------------------------------------------------------------------------- */
/* exponent of p in n! (Legendre)                                               */
static int exp_fact(idx_t n, int p)
{
    int e = 0;
    for (idx_t q = n; q; q /= (idx_t)p)
        e += (int)(q / (idx_t)p);
    return e;
}

/* --------------------------------------------------------------------------- */
/* exponent of p in n!!                                                       */
static int exp_dbl_fact(idx_t n, int p)
{
    if (n <= 0) return 0;
    if ((n & 1) == 0) {                     /* even: n = 2k          */
        idx_t k = n / 2;
        int e = exp_fact(k, p);
        if (p == 2) e += (int)k;            /* extra 2's from 2^k    */
        return e;
    } else {                                 /* odd: n = 2k-1         */
        idx_t k = (n + 1) / 2;              /* k such that 2k-1 = n  */
        int e = exp_fact(2 * k, p)          /* exponent in (2k)!     */
            - ((p == 2) ? (int)k : 0)      /* subtract 2^k          */
            - exp_fact(k, p);              /* subtract k!           */
        return e;
    }
}

/* --------------------------------------------------------------------------- */
/* map linear index n to (row, col)                                            */
static void index_to_rc(idx_t n, idx_t *row, idx_t *col)
{
    /* Solve r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    *row = r;
    *col = n - r * (r + 1) / 2;
}

/* --------------------------------------------------------------------------- */
/* fast exponentiation for seq_t (assumes seq_t supports binary <<)           */
static seq_t pow_seq(seq_t base, int exp)
{
    seq_t res = (seq_t)1;
    while (exp > 0) {
        if (exp & 1) res *= base;
        base *= base;
        exp >>= 1;
    }
    return res;
}

/* --------------------------------------------------------------------------- */
/* main sequence function                                                       */
seq_t a(idx_t n)
{
    if (!primes_built) {
        build_primes();
        primes_built = true;
    }

    idx_t r, c;
    index_to_rc(n, &r, &c);
    seq_t result = (seq_t)1;

    for (size_t i = 0; i < prime_cnt && primes[i] <= r; ++i) {
        int p = primes[i];
        int exp = exp_fact(r, p)
                - exp_dbl_fact(c, p)
                - exp_dbl_fact(r - c, p);
        if (exp > 0)
            result *= pow_seq((seq_t)p, exp);
    }
    return result;
}
