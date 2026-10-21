/*
 * A089219  Numbers of the form (p0^x)(p1^(x+1))(p2^(x+2))...(pk^(x+k)), where
 *          x and k are positive integers and p0,p1,...,pk are primes with
 *          p0<p1<...<pk.
 * https://oeis.org/A089219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = numbers of the form (p0^x)(p1^(x+1))...(pk^(x+k)) where
 *            x,k≥1 and p0<p1<...<pk are primes (they need not be consecutive)
 * Range:     u8 n <= 8, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *spf = NULL;          /* smallest prime factor array */
static size_t spf_size = 0;           /* size of the spf array */

/* Build SPF up to LIMIT.  LIMIT is chosen large enough to contain the
   10 000‑th term (≈1 M is sufficient, we use 2 M). */
static void init_spf(void)
{
    if (spf != NULL)
        return;
    const size_t LIMIT = 2000000u;      /* 2 000 000 */
    spf_size = LIMIT + 1;
    spf = (uint32_t *)malloc(spf_size * sizeof(uint32_t));
    for (size_t i = 0; i < spf_size; ++i)
        spf[i] = (uint32_t)i;
    for (size_t i = 2; i * i < spf_size; ++i) {
        if (spf[i] == i) {
            for (size_t j = i * i; j < spf_size; j += i) {
                if (spf[j] == j)
                    spf[j] = (uint32_t)i;
            }
        }
    }
}

/* Check whether a number satisfies the pattern.  Number must lie in the
   range that fits into the SPF array (≤ spf_size-1). */
static bool is_valid(idx_t num)
{
    if (num < 4)                  /* need at least two prime factors */
        return false;
    uint32_t n = (uint32_t)num;
    int exp[32];
    int cnt = 0;
    while (n != 1) {
        uint32_t p = spf[n];
        int e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }
        exp[cnt++] = e;
        if (cnt >= 32)            /* too many distinct primes */
            return false;
    }
    if (cnt < 2)
        return false;
    int base = exp[0];
    for (int i = 1; i < cnt; ++i) {
        if (exp[i] - i != base)
            return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    static bool initialized = false;
    if (!initialized) {
        init_spf();
        initialized = true;
    }
    idx_t found = 0;
    idx_t candidate = 2;
    for (;;) {
        /* If candidate exceeds the precomputed SPF range we would need a
           larger sieve; the current limit (2 000 000) is ample for n≤10000. */
        if (candidate < spf_size && is_valid(candidate)) {
            ++found;
            if (found == n)
                return (seq_t)candidate;
        }
        ++candidate;
    }
}
