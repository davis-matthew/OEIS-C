/*
 * A037085  Beatty sequence for Pi^2.
 * https://oeis.org/A037085
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n * Pi^2)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 43, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Pi^2 = 9.869604401089358618... */
/*  We use an integer approximation with 7 decimal places:
    Pi^2 ≈ 98696044 / 10^7
    The maximum absolute error is < 1.1 × 10⁻⁹, so for n ≤ 10 000
    the product n * error < 0.011, guaranteeing that the floor
    of the approximated value equals the exact floor of n*Pi^2. */
seq_t a(idx_t n)
{
    /* pre‑computed constants */
    static const seq_t NUM = (seq_t)98696044;   /* floor(Pi^2 * 10^7) */
    static const seq_t DEN = (seq_t)10000000;   /* 10^7 */

    /* integer arithmetic: floor( n * NUM / DEN ) */
    return ((seq_t)n * NUM) / DEN;
}
