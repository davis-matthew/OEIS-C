/*
 * A190440  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(golden ratio,4,0) and
 *          []=floor.
 * https://oeis.org/A190440
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊4 n φ⌋ – 4 ⌊n φ⌋ (φ = golden ratio)
 * Range:     u8 n <= 112, u16 n <= 112, u32 n <= 112, u64 n <= 112,
 *            u128 n <= 112, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The golden ratio is approximated by the convergent 28657/17711,
   which gives an error < 1/17711².  For any n ≤ 10⁴ the error in the
   rounded division is far below 0.5, so the floor operations are
   guaranteed to be correct.  All calculations are done with integer
   arithmetic only. */
seq_t a(idx_t n)
{
    /*  φ ≈ 28657/17711  – convergent of the continued fraction of φ  */
    const uint64_t P = 28657;   /* numerator  */
    const uint64_t Q = 17711;   /* denominator */

    /* Compute floor(n φ)  */
    unsigned __int128 prod = (unsigned __int128)n * P;
    seq_t floor_n_phi = (seq_t)(prod / Q);

    /* Compute floor(4 n φ)  */
    seq_t floor_4n_phi = (seq_t)((prod * 4) / Q);

    /* a(n) = floor(4 n φ) – 4 floor(n φ)  */
    return floor_4n_phi - 4 * floor_n_phi;
}
