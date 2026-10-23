/*
 * A197929  Number of distinct residues of x^(n-1) (mod n), x=0..n-1.
 * https://oeis.org/A197929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of distinct residues of x^(n-1) (mod n) for
 *            x=0..n-1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* not used for OEIS offset but safe */

    /* Allocate a flag array for residues modulo n */
    size_t N = (size_t)n;
    unsigned char *seen = (unsigned char *)malloc(N);
    if (!seen) return (seq_t)0;    /* allocation failure – should not happen */

    memset(seen, 0, N);
    unsigned long long mod = (unsigned long long)n;
    size_t count = 0;

    for (size_t x = 0; x < N; ++x)
    {
        /* fast modular exponentiation */
        unsigned long long base = (unsigned long long)x % mod;
        unsigned long long exp  = N - 1;          /* n-1 <= 9999 for n<=10000 */
        unsigned long long result = 1 % mod;      /* handles mod==1 */

        while (exp > 0)
        {
            if (exp & 1ULL) result = (result * base) % mod;
            base = (base * base) % mod;
            exp >>= 1ULL;
        }

        /* record the residue */
        if (!seen[(size_t)result])
        {
            seen[(size_t)result] = 1;
            ++count;
        }
    }

    free(seen);
    return (seq_t)count;
}
