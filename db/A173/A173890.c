/*
 * A173890  Triangle T(n, k) = c(n)/(c(k)*c(n-k)) where c(n) =
 *          (n-2)!*(n-1)!*n!*(n+1)!*(n+3)!/1440 with c(0) = c(1) = 1 and c(2)
 *          = 2, read by rows.
 * https://oeis.org/A173890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less, easy
 * Formula:   T(n,k)=c(n)/(c(k)*c(n-k)) with c(0)=1,c(1)=1,c(2)=2, and
 *            c(n)=((n-2)!⋯(n+3)!)/1440 for n≥3. a(n) enumerates the triangle
 *            row‑wise starting at offset 0
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 38, u64 n <= 110,
 *            u128 n <= 416, big any n
 * Helpers:   offset_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/offset_row.h"

#define MAX_PRIME_LIMIT 20000
#define MAX_PRIMES      20000   /* enough for primes up to 20000 */

/*------------- prime table -------------------------------------*/
static uint64_t primes[MAX_PRIMES];
static int      prime_index[MAX_PRIME_LIMIT+1];
static int      prime_count;

/* Initialise the prime list once. */
static void init_primes(void)
{
    const int LIMIT = MAX_PRIME_LIMIT;
    for (int i = 0; i <= LIMIT; ++i) prime_index[i] = -1;
    int cnt = 0;
    for (int i = 2; i <= LIMIT; ++i) {
        bool isprime = true;
        for (int d = 2; d * d <= i; ++d) {
            if (i % d == 0) { isprime = false; break; }
        }
        if (isprime) {
            primes[cnt]    = (uint64_t)i;
            prime_index[i] = cnt;
            ++cnt;
        }
    }
    prime_count = cnt;
}

/*------------- row helpers -------------------------------------*/

static idx_t row_from_index(idx_t n)
{
    idx_t low  = 0;
    idx_t high = 1;
    while (offset_row(high) <= n) {
        if (high >= (UINT64_MAX >> 1)) break;
        high <<= 1;
    }
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if (offset_row(mid) <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/*------------- exponent helpers ---------------------------------*/
static int legendre_exp(idx_t m, uint64_t p)
{
    int e = 0;
    while (m) {
        m /= p;
        e += (int)m;
    }
    return e;
}

static void add_exponents_num(idx_t n, int sign, int *exp)
{
    idx_t v[5] = {n - 2, n - 1, n, n + 1, n + 3};
    for (int pi = 0; pi < prime_count; ++pi) {
        uint64_t p = primes[pi];
        int e = 0;
        for (int vi = 0; vi < 5; ++vi) {
            idx_t m = v[vi];
            while (m) {
                m /= p;
                e += (int)m;
            }
        }
        exp[pi] += sign * e;
    }
}

static void add_exponents_c(idx_t n, int sign, int *exp)
{
    /* special cases for n = 0, 1, 2 */
    if (n == 0 || n == 1) return;
    if (n == 2) {
        exp[prime_index[2]] += sign * 1;
        return;
    }
    /* n ≥ 3 : c(n) = num(n) / 1440   */
    add_exponents_num(n, sign, exp);
    /* subtract / add the factor 1440 (=2⁵·3²·5) */
    exp[prime_index[2]] -= sign * 5;
    exp[prime_index[3]] -= sign * 2;
    exp[prime_index[5]] -= sign * 1;
}

/*------------- main sequence function ---------------------------*/
seq_t a(idx_t n)
{
    static bool primes_inited = false;
    if (!primes_inited) {
        init_primes();
        primes_inited = true;
    }

    /* locate row and column */
    idx_t r = row_from_index(n);
    idx_t k = n - offset_row(r);

    /* exponent array */
    static int exp[MAX_PRIMES];
    for (int i = 0; i < prime_count; ++i) exp[i] = 0;

    /* accumulate exponents for numerator and denominators */
    add_exponents_c(r,     +1, exp);
    add_exponents_c(k,     -1, exp);
    add_exponents_c(r - k, -1, exp);

    /* rebuild the integer from prime exponents */
    seq_t result = (seq_t)1;
    for (int pi = 0; pi < prime_count; ++pi) {
        int e = exp[pi];
        if (e > 0) {
            seq_t val = (seq_t)1;
            uint64_t p = primes[pi];
            for (int i = 0; i < e; ++i) val *= (seq_t)p;
            result *= val;
        }
    }
    return result;
}
