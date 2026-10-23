/*
 * A014249  a(n) = b(n) - c(n) where b(n) = [n*sqrt(2)] + [n*sqrt(3)] and c(n)
 *          is the n-th number not in sequence b.
 * https://oeis.org/A014249
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 152, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A014249: a(n) = b(n) – c(n)
    where b(n) = ⌊n√2⌋ + ⌊n√3⌋ and
    c(n) is the n‑th positive integer NOT belonging to the sequence b. */

#include <stdint.h>

/* Integer square root for an unsigned integer type idx_t.
   Returns ⌊√x⌋ without overflow. */
static idx_t isqrt_idx(idx_t x)
{
    idx_t low = 0;
    idx_t high = x;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        /* avoid mid*mid overflow by comparing mid and x/mid */
        if (mid > 0 && mid > x / mid)
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

/* Compute b(k) = floor(k*sqrt(2)) + floor(k*sqrt(3)) as an idx_t. */
static idx_t compute_b_idx(idx_t k)
{
    idx_t kk = k * k;                 /* k²  */
    idx_t f2 = isqrt_idx(2 * kk);     /* ⌊k√2⌋  */
    idx_t f3 = isqrt_idx(3 * kk);     /* ⌊k√3⌋  */
    return f2 + f3;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of domain – not used by OEIS */

    /* Find c(n) – the n-th missing integer from the sequence b. */
    idx_t missing_cnt = 0;          /* how many missing numbers seen so far   */
    idx_t x = 1;                    /* current integer being examined        */
    idx_t k = 1;                    /* current index in sequence b            */
    idx_t b_val = compute_b_idx(k); /* first value of b                       */
    idx_t c_val = 0;                /* will hold c(n) once found             */

    while (1) {
        if (x == b_val) {           /* x is in the sequence b                */
            /* move to next value of b */
            ++k;
            b_val = compute_b_idx(k);
            ++x;
            continue;
        }
        /* x is missing from b */
        ++missing_cnt;
        if (missing_cnt == n) {
            c_val = x;
            break;
        }
        ++x;
    }

    /* Compute b(n) directly. */
    idx_t b_n = compute_b_idx(n);

    /* Result is b(n) – c(n).  Use idx_t for the subtraction
       then cast to seq_t – the result must fit in seq_t. */
    idx_t a_val = b_n - c_val;
    return (seq_t)a_val;
}
