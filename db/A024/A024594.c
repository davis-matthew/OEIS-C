/*
 * A024594  s(1)t(n) + s(2)t(n-1) + ... + s(k)t(n+1-k), where k = [ (n+1)/2 ],
 *          s = (F(2), F(3), ...), t = A001950 (upper Wythoff sequence).
 * https://oeis.org/A024594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1}^{⌊(n+1)/2⌋} F(k+1) * ( (n+1-k) + ⌊(n+1-k)·φ⌋ )
 *            where F(m) is the Fibonacci sequence with F(0)=0, F(1)=1, and φ
 *            = (1+√5)/2
 * Range:     u8 n <= 8, u16 n <= 27, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>


/* Integer square‑root of a 64‑bit unsigned integer, using binary search. */
static inline idx_t isqrt_u64(idx_t v)
{
    idx_t lo = 0;
    idx_t hi = v;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        idx_t sq  = mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Compute ⌊n·φ⌋ using only integer arithmetic. */
static inline idx_t floor_n_phi(idx_t n)
{
    /* ⌊n·φ⌋ = (n + ⌊n·√5⌋)/2  */
    idx_t sqrt_term = isqrt_u64(5 * n * n);   /* ⌊n·√5⌋ */
    return (n + sqrt_term) >> 1;              /* divide by 2 */
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;                /* undefined, but safe */

    idx_t kmax = (n + 1) >> 1;                  /* ⌊(n+1)/2⌋ */

    /* Fibonacci state: fib_prev = F(k-1), fib_curr = F(k) */
    seq_t fib_prev = (seq_t)0;                  /* F(0) */
    seq_t fib_curr = (seq_t)1;                  /* F(1) */

    seq_t result = (seq_t)0;

    for (idx_t k = 1; k <= kmax; ++k) {
        /* s_k = F(k+1) */
        seq_t s_val = fib_prev + fib_curr;

        /* t_{n+1-k} = (n+1-k) + ⌊(n+1-k)·φ⌋ */
        idx_t t_idx = n + 1 - k;
        idx_t phi    = floor_n_phi(t_idx);
        seq_t t_val  = (seq_t)(t_idx + phi);

        result += s_val * t_val;

        /* Advance Fibonacci sequence */
        fib_prev = fib_curr;
        fib_curr = s_val;
    }

    return result;
}
