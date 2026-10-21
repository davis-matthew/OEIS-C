/*
 * A081244  Named periods in the Mayan/Mesoamerican calendars.
 * https://oeis.org/A081244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(0)=1, a(1)=20, a(2)=260, a(3)=360, a(4)=365, a(5)=7200,
 *            a(6)=18980, a(n)=7200*20^(n-6) for n>=7
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 10, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return 1;
    if (n == 1) return 20;
    if (n == 2) return 260;
    if (n == 3) return 360;
    if (n == 4) return 365;
    if (n == 5) return 7200;
    if (n == 6) return 18980;

    /* n >= 7 : a(n) = 7200 * 20^(n-6) */
    seq_t val = 7200;          /* a(5) */
    idx_t exp = n - 6;         /* power of 20 to apply */
    while (exp > 0) {
        val *= 20;
        --exp;
    }
    return val;
}
