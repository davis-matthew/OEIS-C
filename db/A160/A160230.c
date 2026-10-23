/*
 * A160230  a(n) = A004760(n+1)-A160217(n), n>=1.
 * https://oeis.org/A160230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 0, followed by a(2n) = a(2n+1) = 2*a(n) if v2(n) is even
 *            a(2n) = 2*a(n)+3, a(2n+1)=2*a(n)+2 if v2(n) is odd where v2(n)
 *            is the exponent of the highest power of 2 dividing n
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count trailing zeros of a non‑zero 64‑bit integer. */
static unsigned int v2_trailing_zeros(idx_t x)
{
    unsigned int tz = 0;
    /* x is guaranteed to be >0 when this function is called. */
    while ((x & 1ULL) == 0ULL) {
        ++tz;
        x >>= 1;
    }
    return tz;
}

/* Recursive helper that evaluates a(n) using the recurrence. */
static seq_t a_rec(idx_t n)
{
    if (n <= 1)
        return 0;        /* a(1) = 0 */

    idx_t parent = n >> 1;               /* floor(n/2) */
    seq_t base  = a_rec(parent);         /* a(parent) */

    unsigned int tz = v2_trailing_zeros(parent);   /* v2(parent) */

    if ((tz & 1) == 0) {                 /* even v2(parent): both children 2*base */
        return base << 1;
    } else {                              /* odd v2(parent): children differ */
        if ((n & 1ULL) == 0ULL)          /* child 2*parent (even index) */
            return (base << 1) + (seq_t)3;
        else                             /* child 2*parent+1 (odd index) */
            return (base << 1) + (seq_t)2;
    }
}

/* Public entry point. */
seq_t a(idx_t n)
{
    return a_rec(n);
}
