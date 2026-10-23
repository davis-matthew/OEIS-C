/*
 * A123587  Number of bars that must change on a standard digital clock when
 *          the minutes change from n-1 to n.
 * https://oeis.org/A123587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, base, dumb
 * Formula:   a(n) = number of segment changes from minute (n-1) mod 60 to
 *            minute n mod 60 on a standard 7‑segment display
 * Range:     u8 n <= 119, u16 n <= 119, u32 n <= 119, u64 n <= 119,
 *            u128 n <= 119, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 7‑segment masks for digits 0–9.  Bits: 0‑6 == a‑g. */
    static const unsigned seg[10] = {
        0x3F, /* 0: 0b0111111  a,b,c,d,e,f */
        0x06, /* 1: 0b0000110  b,c */
        0x5B, /* 2: 0b1011011  a,b,g,e,d */
        0x4F, /* 3: 0b1001111  a,b,g,c,d */
        0x66, /* 4: 0b1100110  f,g,b,c */
        0x6D, /* 5: 0b1101101  a,f,g,c,d */
        0x7D, /* 6: 0b1111101  a,f,g,e,c,d */
        0x07, /* 7: 0b0000111  a,b,c */
        0x7F, /* 8: 0b1111111  a,b,c,d,e,f,g */
        0x6F  /* 9: 0b1101111  a,b,c,g,f,d */
    };

    /* Current minute (mod 60) and previous minute. */
    idx_t curr = n % 60;
    idx_t prev = (n + 59) % 60;          /* (n-1) mod 60 */

    /* Decode tens and units digits. */
    unsigned d1 = prev / 10;   /* tens of (n-1) */
    unsigned d2 = prev % 10;   /* units of (n-1) */
    unsigned e1 = curr / 10;   /* tens of n   */
    unsigned e2 = curr % 10;   /* units of n   */

    /* Count segment differences for each digit. */
    int changes = __builtin_popcount((unsigned)(seg[d1] ^ seg[e1])) +
                  __builtin_popcount((unsigned)(seg[d2] ^ seg[e2]));

    return (seq_t)changes;
}
