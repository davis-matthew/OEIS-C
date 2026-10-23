/*
 * A104798  Triangle T(n,k) = k*[Fib(n-k+3) - 1], read by rows.
 * https://oeis.org/A104798
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = k*[Fib(n-k+3)-1]
 * Range:     u8 n <= 57, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle rows: T(n, k) = k * (Fib(n-k+3) - 1); offset 2 (a(2)=T(1,1)). */

#include <stdbool.h>

static inline idx_t int_sqrt_idx(idx_t x)
{
    /* binary search for floor(sqrt(x)) without overflow */
    idx_t lo = 0, hi = x;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= x / mid) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

static seq_t fib_arr[151];
static bool fib_init = false;

/* Initialize Fibonacci array up to index 150 */
static void init_fib(void)
{
    if (!fib_init) {
        fib_init = true;
        fib_arr[0] = 0;
        fib_arr[1] = 1;
        for (idx_t i = 2; i < 151; ++i)
            fib_arr[i] = fib_arr[i - 1] + fib_arr[i - 2];
    }
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;          /* values below offset are arbitrary */

    init_fib();

    /* step 1: find the zero‑based position p in the triangle */
    idx_t p = n - 2;

    /* step 2: compute row r with r*(r-1)/2 <= p < r*(r+1)/2
       r = floor((1 + sqrt(1 + 8p))/2)                               */
    idx_t disc   = 8 * p + 1;                      /* 8p+1 fits in idx_t for allowed n */
    idx_t row    = (1 + int_sqrt_idx(disc)) / 2;
    idx_t col    = p - row * (row - 1) / 2 + 1;   /* column, 1‑based */

    /* step 3: compute Fibonacci value needed */
    idx_t fib_idx = row - col + 3;
    seq_t f = fib_arr[fib_idx];

    /* step 4: compute the triangle entry and return */
    return col * (f - 1);
}
