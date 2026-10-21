/*
 * A055528  a(n) = 10*a(n-1)+n^3, a(0)=0.
 * https://oeis.org/A055528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10*a(n-1) + n^3,  a(0) = 0
 * This implementation builds the value by iterating up to n.
 * It works for all supported ⟨seq_t, idx_t⟩ combinations,
 * including arbitrary‑precision builds (where seq_t is a class).
 *
 * No dynamic memory is needed; the routine is O(n) and trivially
 * memofree for all builds.
 */
seq_t a(idx_t n)
{
    seq_t val = (seq_t)0;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t cast_i = (seq_t)i;
        seq_t cube  = cast_i * cast_i * cast_i;
        val = val * (seq_t)10 + cube;
    }
    return val;
}
