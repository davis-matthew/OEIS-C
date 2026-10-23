/*
 * A178569  a(2*n) = 10*a(n), a(2*n+1) = a(n) + a(n+1).
 * https://oeis.org/A178569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(2*n) = 10*a(n), a(2*n+1) = a(n)+a(n+1), a(1)=1
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define MAX_CACHE 65536u
static seq_t cache[MAX_CACHE];   /* zero‑initialized; used for memoization */

static seq_t compute_a(idx_t n)
{
    if (n == 1)            /* base case */
        return 1;

    if (n < MAX_CACHE && cache[n] != 0)
        return cache[n];

    seq_t val;
    if ((n & 1u) == 0u) {          /* even: a(2*k) = 10*a(k) */
        val = 10 * compute_a(n >> 1);
    } else {                       /* odd:  a(2*k+1) = a(k)+a(k+1) */
        val = compute_a(n >> 1) + compute_a((n >> 1) + 1);
    }

    if (n < MAX_CACHE)
        cache[n] = val;

    return val;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                  /* undefined, safe return value */
    return compute_a(n);
}
