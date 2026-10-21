/*
 * A090284  "Plain Bob Minimus" in bell-ringing is a sequence of permutations
 *          p_1=(1,2,3,4), p_2=(2,1,4,3), .. which runs through all
 *          permutations of {1,2,3,4} with period 24; sequence gives position
 *          of bell 4 (the tenor bell) in n-th permutation.
 * https://oeis.org/A090284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   24‑periodic(sequence) a(1)=4, and for all n≥1, a(n) =
 *            period[(n-1) mod 24], with period =
 *            4,3,2,1,1,2,3,4,3,4,4,3,2,1,1,2,2,1,1,2,3,4,4,3
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t period[24] = {
        4, 3, 2, 1, 1, 2, 3, 4,
        3, 4, 4, 3, 2, 1, 1, 2,
        2, 1, 1, 2, 3, 4, 4, 3
    };
    /* n is 1‑based; convert to 0‑based index modulo 24 */
    idx_t idx = (n - 1ULL) % 24ULL;
    return period[idx];
}
