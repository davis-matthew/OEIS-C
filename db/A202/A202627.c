/*
 * A202627  Order of Fibonacci group F(n,5) (or 0 if the group is infinite).
 * https://oeis.org/A202627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 11 if n = 2, * 22 if n = 3, * 4 if n = 5, * 7775 if n =
 *            6, * 0 otherwise (group is infinite)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The function is O(1) and works for any order of calls. */
    switch((uint64_t)n)
    {
        case 2: return 11;
        case 3: return 22;
        case 5: return 4;
        case 6: return 7775;
        default: return 0;
    }
}
