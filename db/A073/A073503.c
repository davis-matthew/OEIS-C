/*
 * A073503  Numbers n such that the number of solutions to x^4 == 1 (mod n) is
 *          twice the number of solutions of x^2 == 1 (mod n).
 * https://oeis.org/A073503
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 123, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A073503: Numbers n such that the number of solutions to
   x^4 ≡ 1 (mod n) is twice the number of solutions to x^2 ≡ 1 (mod n).
   The function builds the sequence by scanning integers and using a
   smallest‑prime‑factor sieve to test the condition. */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* maximum value that will be inspected.   1 000 000 is far bigger than
   needed for n ≤ 10 000 (where a(n) ≈ 2 n). */
#define LIMIT 1000000U

/* smallest prime factor table */
static uint32_t *spf = NULL;
static idx_t spf_size = 0;

/* Builds the smallest‑prime‑factor table up to LIMIT.  */
static void build_spf(void)
{
    if (spf) return;                     /* already built */
    spf = (uint32_t *)malloc((LIMIT + 1) * sizeof(uint32_t));
    if (!spf) exit(1);
    for (idx_t i = 0; i <= LIMIT; ++i) spf[i] = 0;
    spf[0] = spf[1] = 1;
    for (uint32_t i = 2; (uint64_t)i * i <= LIMIT; ++i)
    {
        if (spf[i] != 0) continue;
        spf[i] = i;
        for (uint32_t j = i * i; j <= LIMIT; j += i)
            if (spf[j] == 0) spf[j] = i;
    }
    for (uint32_t i = 2; i <= LIMIT; ++i)
        if (spf[i] == 0) spf[i] = i;
    spf_size = LIMIT + 1;
}

/* Returns true if n satisfies the property.  n is assumed <= LIMIT. */
static bool satisfies(uint64_t n)
{
    if (n < 2) return false;          /* 1 is not in the sequence */

    int two_factors = 0;              /* how many prime powers give factor 2 */
    uint64_t x = n;
    while (x > 1)
    {
        uint32_t p = spf[x];
        int e = 0;
        while (x % p == 0) { x /= p; ++e; }

        if (p == 2)
        {
            if (e >= 4) ++two_factors;
        }
        else if (p % 4 == 1)
        {
            ++two_factors;
        }
        /* primes≡3 (mod 4) contribute nothing */

        if (two_factors > 1) return false;
    }
    return two_factors == 1;
}

/* dynamic array to store the found terms */
static seq_t *terms = NULL;
static idx_t terms_len = 0;
static idx_t terms_cap = 0;

/* returns the nth term of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, but graceful */

    /* ensure the sieve exists */
    if (!spf) build_spf();

    /* generate terms on demand */
    static uint64_t last_checked = 1;
    while (terms_len < n)
    {
        ++last_checked;
        if (satisfies(last_checked))
        {
            if (terms_len == terms_cap)
            {
                size_t new_cap = terms_cap ? terms_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(terms, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);
                terms = tmp;
                terms_cap = new_cap;
            }
            terms[terms_len++] = (seq_t)last_checked;
        }
    }
    return terms[n - 1];
}
