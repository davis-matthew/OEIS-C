/*
 * A081134  Distance to nearest power of 3.
 * https://oeis.org/A081134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = min(n-3^⌊log₃(n)⌋ , 3^(⌊log₃(n)⌋+1)-n)
 * Range:     u8 n <= 416, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* i.e. the distance from n to the nearest (lower or upper) power of 3. */

seq_t a(idx_t n)
{
    /* Treat the (offset‑1) case as 0; the official offset is 1. */
    if (n == 0)
        return 0;

    /* Find the largest power of 3 not exceeding n. */
    seq_t power = 1;                        /* 3^k  (k >= 0) */
    while (1) {
        seq_t next = power * (seq_t)3;
        /* Detect overflow by checking wrap‑around: if next < power, overflow occurred. */
        if (next < power)
            break;                         /* cannot safely multiply further */
        if (next > (seq_t)n)
            break;                         /* next would exceed n */
        power = next;
    }

    seq_t down = (seq_t)n - power;          /* distance to lower power of 3 */

    /* Compute distance to the next higher power of 3, if it can be represented. */
    seq_t up;
    seq_t nextPower = power * (seq_t)3;
    if (nextPower < power) {                /* overflow – next power is too large */
        /* In this case the upper distance is effectively infinite; return down. */
        return down;
    }
    up = nextPower - (seq_t)n;              /* distance to upper power of 3 */

    /* Return the smaller distance. */
    return (down < up) ? down : up;
}
