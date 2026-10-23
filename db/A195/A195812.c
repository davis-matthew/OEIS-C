/*
 * A195812  Sum of the distinct residues of x^n (mod n), x=0..n-1.
 * https://oeis.org/A195812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum of the distinct residues of x^n (mod n), x = 0..n-1
 * Range:     u8 n <= 28, u16 n <= 364, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* fast modular exponentiation: return (base^exp) mod mod */
static inline seq_t mod_pow(seq_t base, idx_t exp, seq_t mod)
{
    if (mod == 1) return 0;
    seq_t result = 1 % mod;
    seq_t b = base % mod;
    while (exp)
    {
        if (exp & 1U)
            result = (result * b) % mod;
        b = (b * b) % mod;
        exp >>= 1U;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* static buffer for seen residues, reused across calls */
    static char *seen = NULL;
    static idx_t seen_size = 0;

    if (n > seen_size)
    {
        /* Release the old buffer and allocate a larger one. */
        free(seen);
        seen = (char*)malloc((size_t)n);
        seen_size = n;
    }

    /* Mark all residues as unseen. */
    memset(seen, 0, (size_t)n);

    /* Compute residues and mark the distinct ones. */
    for (idx_t x = 0; x < n; ++x)
    {
        seq_t r = mod_pow(x, n, n);
        /* r is between 0 and n-1, so it fits in the seen array. */
        seen[(size_t)r] = 1;
    }

    /* Sum the distinct residues. */
    seq_t sum = 0;
    for (idx_t r = 0; r < n; ++r)
    {
        if (seen[(size_t)r])
            sum += r;
    }

    return sum;
}
