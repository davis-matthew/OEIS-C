/*
 * A109648  Sequence is its own ASCII representation (including commas and
 *          spaces).
 * https://oeis.org/A109648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 53 a(4n) = 48 + floor( a(n)/10 ) a(4n+1) = 48 + ( a(n)
 *            mod 10 ) a(4n+2) = 44 a(4n+3) = 32
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 68 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t rec(idx_t n)
{
    /* Recursive evaluation based on the recurrence.  The recursion
       depth is `O(log n)`, since each step divides the index by 4.  */
    if (n == 0)
        return (seq_t)53;              /* base case a(0) */

    unsigned r = (unsigned)(n & 3);    /* n % 4 using 2‑bit mask */

    /* Handles the two constant cases directly. */
    if (r == 2) return (seq_t)44;
    if (r == 3) return (seq_t)32;

    /* For r == 0 or r == 1 we need the value at index n/4. */
    seq_t sub = rec(n >> 2);          /* equivalent to rec(n/4) */

    if (r == 0)  /* a(4n) = 48 + floor(sub/10) */
        return (seq_t)48 + (sub / (seq_t)10);
    else         /* a(4n+1) = 48 + (sub % 10) */
        return (seq_t)48 + (sub % (seq_t)10);
}

seq_t a(idx_t n)
{
    return rec(n);
}
