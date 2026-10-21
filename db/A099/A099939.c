/*
 * A099939  Seven-segment LED clock sequence: starting at 00:00 midnight,
 *          sequence gives total number of illuminated segments every minute
 *          that appear in the hour and minute characters of the LED display
 *          on a digital clock.
 * https://oeis.org/A099939
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Seven‑segment LED clock sequence: 
   a(n) = number of segments illuminated on a 12‑hour digital clock
   starting at 12:00 AM (n=0). The clock shows the hour without a leading
   zero (12,1,…,11) and the minute with two digits (00–59). 
*/
seq_t a(idx_t n)
{
    /* segment counts for digits 0–9 */
    static const unsigned char seg[10] =
        {6, 2, 5, 5, 4, 5, 6, 3, 7, 6};

    /* minute part (always two digits) */
    int minute   = (int)(n % 60);
    int mn_tens  = minute / 10;
    int mn_ones  = minute % 10;

    /* hour part in 12‑hour format */
    idx_t hourIndex = (n / 60) % 12;          /* 0→12, 1→1, …, 11→11 */
    int hour = (hourIndex == 0) ? 12 : (int)hourIndex;
    int hr_tens  = hour / 10;   /* used only when hour ≥ 10 */
    int hr_ones  = hour % 10;

    /* sum segments */
    seq_t total = seg[hr_ones]               /* hour digit(s) */
                 + seg[mn_tens] + seg[mn_ones];
    if (hour >= 10)
        total += seg[hr_tens];

    return total;
}
