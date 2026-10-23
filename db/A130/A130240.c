/*
 * A130240  Partial sums of A130239.
 * https://oeis.org/A130240
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1) * A130233(floor(sqrt(n))) –
 *            Fib(A130233(floor(sqrt(n))) + 1) * Fib(A130232(floor(sqrt(n))))
 *            A130233(j) = largest Fibonacci index ≤ j A130232(j) = same as
 *            A130233(j) Thus a(n) can be computed as: let r = floor(sqrt(n))
 *            let s = largest Fibonacci index whose value ≤ r a(n) = (n+1) * s
 *            –...
 * Range:     u8 n <= 58, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute floor of the integer square root of an unsigned 64‑bit number.
   No floating point or math.h is used. */
static idx_t floor_sqrt_u64(idx_t n)
{
    idx_t lo = 0, hi = (n < (idx_t)4294967297ULL) ? n : (idx_t)4294967297ULL;
    /* hi is > sqrt(max unsigned 64‑bit) so hi will be reduced below that */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) {
            lo = 1;
            continue;
        }
        if (mid > n / mid) {
            hi = mid - 1;
        } else {
            /* check if mid+1 would overflow or exceed n */
            if ((mid + 1) > n / (mid + 1))
                return mid;
            lo = mid + 1;
        }
    }
    return hi;
}

/* The following function contains static data shared across calls. */
seq_t a(idx_t n)
{
    /* Thirty‑six Fibonacci numbers comfortably exceed 2^32, which is
       the maximum possible sqrt(n) for unsigned 64‑bit n. We pre‑compute
       them in a static array to avoid repeated work. */
    static unsigned long long fib[90];
    static int fib_count = 0;
    if (fib_count == 0) {
        fib[0] = 0ULL;
        fib[1] = 1ULL;
        fib_count = 2;
        while (fib_count < 90) {
            unsigned long long next = fib[fib_count - 1] + fib[fib_count - 2];
            /* Stop if we are about to overflow 64‑bit unsigned. */
            if (next < fib[fib_count - 1]) break;
            fib[fib_count++] = next;
        }
    }

    /* The answer for n = 0 is 0. */
    if (n == 0) return (seq_t)0;

    idx_t r = floor_sqrt_u64(n);

    /* Find the largest index s with fib[s] <= r. */
    int s = 0;
    while (s + 1 < fib_count && fib[s + 1] <= r) s++;

    /* Ensure we have fib[s+1] (may exceed r). */
    if (s + 1 >= fib_count) {
        /* Compute one more Fibonacci if not already present. */
        unsigned long long next = fib[s] + fib[s - 1];
        if (next >= fib[s]) {
            if (s + 1 < 90) fib[s + 1] = next;
            else { /* Should not happen for the range of n we handle. */ }
        }
    }

    seq_t n_seq = (seq_t)n;
    seq_t term1  = (n_seq + 1) * (seq_t)s;
    seq_t fib_s  = (seq_t)fib[s];
    seq_t fib_s1 = (seq_t)fib[s + 1];
    seq_t term2  = fib_s1 * fib_s;
    return term1 - term2;
}
