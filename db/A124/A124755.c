/*
 * A124755  Binomial sum of compositions in standard order.
 * https://oeis.org/A124755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabf
 * Formula:   a(n) is the weighted sum of the n‑th composition in standard
 *            order (OEIS A066099). For the empty composition a(0)=0. For a
 *            composition b(1)…b(k), a = Σ_{i=1}^{k} C(k-1,i-1) * b(i)
 *            (binomial coefficients, left to right)
 * Range:     u8 n <= 510, u16 n <= 9806, u32 n <= 9806, u64 n <= 9806,
 *            u128 n <= 9806, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  We map the index n to a composition of some integer m:
        - The compositions are listed as follows:  (m=0) empty,
          then all compositions of 1, all of 2, all of 3, … .
          The number of compositions of m (m>0) is 2^{m-1}.
          Thus the total compositions up to m (including empty) = 2^m.
        - For n>0  let m = floor(log2(n)) + 1.
          Then the n-th composition lies among those of value m
          and has offset pos = n - 2^{m-1}.
        - The offset pos (0‑based) corresponds to a binary string of length m‑1
          (the standard “cut‑bit” representation).  Reading the bits from
          most significant to least gives the composition parts.

   The function below implements this mapping, builds the parts array
   and evaluates the weighted sum using the supplied binomial routine
   from utils/binom.h.  All integer math is done with the build‑defined
   seq_t type.  */

#include <stdint.h>
#include "../utils/binom.h"

/* Compute floor(log2(v)) for v > 0.  Only integer shifts are used. */
static inline idx_t floor_log2_uint64(idx_t v)
{
    idx_t r = 0;
    while (v >>= 1) r++;
    return r;
}

seq_t a(idx_t n)
{
    /* Special case for the empty composition. */
    if (n == 0)
        return 0;

    /* Determine the integer m whose compositions contain the n‑th entry. */
    idx_t m = floor_log2_uint64(n) + 1;          /* m ≥ 1 */

    /* Starting index of compositions of m (0‑based). */
    idx_t start = ((idx_t)1) << (m - 1);

    /* Offset within the block of compositions of m. */
    idx_t pos = n - start;

    /* Build the composition parts from the binary representation of pos. */
    /* At most m parts (one for each 1 in pos + 1). */
    idx_t parts[64];          /* 64 is enough for 64‑bit idx_t. */
    idx_t len = 0;            /* number of parts */
    idx_t part_len = 1;       /* current part length */

    /* There are m-1 cut bits.  Process from most significant bit to least. */
    for (idx_t i = 0; i < m - 1; ++i) {
        idx_t bit = (pos >> (m - 2 - i)) & 1ULL;
        if (bit) {            /* cut after this position */
            parts[len++] = part_len;
            part_len = 1;
        } else {
            ++part_len;
        }
    }
    parts[len++] = part_len;    /* final part */

    /* Compute the weighted sum with binomial coefficients. */
    seq_t result = 0;
    for (idx_t i = 0; i < len; ++i) {
        seq_t weight = binom(len - 1, i);
        result += weight * (seq_t)parts[i];
    }

    return result;
}
