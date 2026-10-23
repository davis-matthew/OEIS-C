/*
 * A135064  Numbers n such that the quintic polynomial x^5 - 10*n*x^2 - 24*n
 *          has Galois group A_5 over rationals.
 * https://oeis.org/A135064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Lucas number L_{2n+1} for n >= 2; a(1) = 1. * * Lucas
 *            numbers L(k) satisfy L(0) = 2, L(1) = 1, and * L(k) = L(k-1) +
 *            L(k-2) for k >= 2. * The sequence A135064 consists of those
 *            Lucas numbers whose index is * 2n+1 (n >= 2), with the first
 *            term being L(1)=1. * We memoise the Lucas values up...
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t lucas[20002] = {0};  /* enough for n <= 10000 (index up to 20001) */
static idx_t lucas_len = 0;       /* number of Lucas values that have been computed */

seq_t a(idx_t n)
{
    /* special case for the first term */
    if (n == 1)
        return (seq_t)1;

    /* initialise the table on the first call */
    if (lucas_len == 0) {
        lucas[0] = (seq_t)2;
        lucas[1] = (seq_t)1;
        lucas_len = 2;
    }

    /* compute the Lucas index needed for this term */
    idx_t k = 2 * n + 1;
    if (k >= lucas_len) {
        /* extend the memoised table up to index k */
        for (idx_t i = lucas_len; i <= k; ++i) {
            lucas[i] = lucas[i - 1] + lucas[i - 2];
        }
        lucas_len = k + 1;
    }

    return lucas[k];
}
