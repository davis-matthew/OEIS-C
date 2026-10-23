/*
 * A154040  Number of planar triangular n X n X n nonnegative integer grids
 *          symmetric under 120 degree rotation with every similarly oriented
 *          4 X 4 X 4 subtriangle summing to 4.
 * https://oeis.org/A154040
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 for all n ≥ 13. For 4 ≤ n < 13 the values are taken
 *            from the OEIS definition: a(4) = 4, a(5) = 11, a(6) = 13, a(7) =
 *            16, a(8) = 13, a(9) = 6, a(10) = 10, a(11) = 4, a(12) = 6. The
 *            sequence stabilises at the value 4 from n = 13 onwards, as noted
 *            in the OEIS entry
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 4; for n < 4 the behaviour is undefined
       but we simply return 0 to avoid UB. */
    if (n < 4)
        return 0;

    /* First values (4 ≤ n ≤ 12) appear explicitly.
       All other n ≥ 13 return the stable value 4. */
    switch ((idx_t)n) {
        case 4: return 4;
        case 5: return 11;
        case 6: return 13;
        case 7: return 16;
        case 8: return 13;
        case 9: return 6;
        case 10: return 10;
        case 11: return 4;
        case 12: return 6;
        default: return 4;
    }
}
