/*
 * A060513  Number of units of time making up the next larger unit: seconds,
 *          minutes, hours, days, years, centuries, millennia.
 * https://oeis.org/A060513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the number of smaller units in the next larger time unit.
   a(1)=60, a(2)=60, a(3)=24, a(4)=365, a(5)=100, a(6)=10. */
seq_t a(idx_t n)
{
    /* No additional state is required; the values are fixed by definition. */
    switch (n) {
        case 1: return (seq_t)60;  /* seconds → minutes */
        case 2: return (seq_t)60;  /* minutes → hours   */
        case 3: return (seq_t)24;  /* hours   → days    */
        case 4: return (seq_t)365; /* days    → years   */
        case 5: return (seq_t)100; /* years   → centuries */
        case 6: return (seq_t)10;  /* centuries → millennia */
        default: return (seq_t)0;  /* undefined beyond 6 */
    }
}
