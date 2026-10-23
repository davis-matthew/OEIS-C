/*
 * A182158  Number of solutions to x^2 + x + y + y^2 == 1 mod n.
 * https://oeis.org/A182158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) is the number of ordered pairs (x,y) modulo n that satisfy
 *            * x^2 + x + y + y^2 ≡ 1 (mod n). The algorithm computes f(z) =
 *            z^2 + z * for all residues z, stores their frequencies, and then
 *            counts pairs * whose f-values sum to 1 modulo n. Complexity:
 *            O(n)
 * Range:     u8 n <= 216, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety, though n ≥ 1 */

    /* Use size_t for array indexing */
    size_t N = (size_t)n;
    seq_t result = 0;

#ifdef __cplusplus
    std::vector<seq_t> freq(N);           /* frequency of each f(z) */
    std::vector<seq_t> farr(N);           /* f(z) values for each z   */
#else
    seq_t *freq = (seq_t *)calloc(N, sizeof(seq_t));   /* init to 0 */
    seq_t *farr = (seq_t *)malloc(N * sizeof(seq_t));
    if (!freq || !farr) return 0;        /* allocation failure */
#endif

    /* First pass: compute f(z) for all z and fill freq */
    for (idx_t z = 0; z < n; ++z)
    {
        uint64_t zz   = (uint64_t)z;
        uint64_t ftmp = zz * zz + zz;                 /* z^2 + z   */
        uint64_t fmod = ftmp % (uint64_t)n;          /* mod n      */
        seq_t fx = (seq_t)fmod;
        farr[z] = fx;
#ifdef __cplusplus
        ++freq[(size_t)fmod];
#else
        ++freq[(size_t)fmod];
#endif
    }

    /* Second pass: sum frequencies for the complementary value */
    for (idx_t z = 0; z < n; ++z)
    {
        uint64_t need = ((uint64_t)1 + (uint64_t)n - (uint64_t)farr[z]) % (uint64_t)n;
#ifdef __cplusplus
        result += freq[(size_t)need];
#else
        result += freq[(size_t)need];
#endif
    }

#ifndef __cplusplus
    free(farr);
    free(freq);
#endif

    return result;
}
