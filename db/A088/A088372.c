/*
 * A088372  Table read by rows where T(0,0)=0; n-th row has 2^n terms
 *          T(n,j),j=0 to 2^n-1. T(n,T088208(n,j))=2^n-j, where T088208 is the
 *          table described in A088208.
 * https://oeis.org/A088372
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = read the table by rows where a(0) = 0; row r (r ≥ 1) has
 *            2^r terms T(r,j) = 2^r – g⁻¹(reverse_bits(j, r)), with g⁻¹ the
 *            inverse Gray‑code mapping
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   gray_to_binary
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gray_to_binary.h"
/*--- bit utilities -------------------------------------------------------*/
static inline idx_t floor_log2(idx_t v)
{
    /* v > 0 */
    idx_t r = 0;
    while (v >>= 1)
        ++r;
    return r;
}

static inline idx_t reverse_bits(idx_t x, idx_t bits)
{
    idx_t rev = 0;
    for (idx_t i = 0; i < bits; ++i) {
        rev = (rev << 1) | (x & 1ULL);
        x >>= 1;
    }
    return rev;
}

/*--- the required sequence function -------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* row index r and offset of the first element of that row */
    idx_t r      = floor_log2(n + 1);
    idx_t pow2   = ((idx_t)1) << r;          /* 2^r as idx_t          */
    idx_t offset = pow2 - 1;                 /* first index of row r  */
    idx_t j      = n - offset;                /* position inside row   */

    /* inverse permutation: P(j) = g⁻¹(reverse_bits(j, r)) */
    idx_t rev  = reverse_bits(j, r);
    idx_t inv  = gray_to_binary(rev, r);

    /* 2^r as seq_t */
    seq_t pow2_seq = ((seq_t)1) << r;

    return pow2_seq - (seq_t)inv;
}
