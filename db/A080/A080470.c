/*
 * A080470  a(n) = smallest composite number that is obtained by placing
 *          digits anywhere in n; a(n) = n if n is composite.
 * https://oeis.org/A080470
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest composite obtainable by inserting arbitrary
 *            decimal digits into the decimal representation of n (preserving
 *            the order of the original digits). If n itself is composite then
 *            a(n)=n
 * Range:     u8 n <= 66, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "../utils/isPrime.h"

/* test whether string sub is a subsequence of string full */
static bool subseq(const char *sub, const char *full)
{
    while (*sub && *full) {
        if (*sub == *full) sub++;
        full++;
    }
    return !*sub;
}

/* find the smallest composite number that contains the digits
   of n as a subsequence.  n is known to fit in idx_t. */
static seq_t find_min_composite(idx_t n)
{
    seq_t base = (seq_t)n;
    /* If n is already composite return it. */
    if (base > (seq_t)1 && !isPrime((idx_t)base))
        return base;

    /* convert n to decimal string once */
    char subbuf[32];
    snprintf(subbuf, sizeof(subbuf), "%llu", (unsigned long long)n);

    for (seq_t m = base + 1; ; ++m) {
        if (m > (seq_t)1 && !isPrime((idx_t)m)) {
            char buf[32];
            snprintf(buf, sizeof(buf), "%llu", (unsigned long long)m);
            if (subseq(subbuf, buf))
                return m;
        }
    }
}

/* sequence function required by the test harness */
seq_t a(idx_t n)
{
    return find_min_composite(n);
}
