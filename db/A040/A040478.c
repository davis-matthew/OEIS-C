/*
 * A040478  Continued fraction for sqrt(501).
 * https://oeis.org/A040478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Continued fraction partial quotients of sqrt(501)
   a(0) = floor(sqrt(501)) = 22
   For n ≥ 1, a(n) is the (n‑th term of the periodic part).  */

#include <stdbool.h>
#include <stdint.h>

/*---------------------------------------*/
/* integer square root (floor) for idx_t */
/*---------------------------------------*/
static idx_t floor_isqrt(idx_t x)
{
    if (x < 2)
        return x;
    idx_t lo = 0, hi = (x >> 1) + 1;
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo + 1) >> 1);
        if (mid > 0 && mid > x / mid)
            hi = mid - 1;
        else
            lo = mid;
    }
    return lo;
}

/*-------------------------------------------*/
/* periodic part of the continued fraction  */
/*-------------------------------------------*/
static idx_t period[128];          /* storage for the periodic terms (a1 … ap) */
static idx_t period_len = 0;       /* the real length of the period        */
static idx_t a0 = 0;               /* floor(sqrt(501))                     */
static bool initialized = false;   /* indicate whether init_period has run */

static void init_period(void)
{
    const idx_t D = 501;
    a0 = floor_isqrt(D);           /* 22 */

    idx_t m = 0;
    idx_t d = 1;
    idx_t a_prev = a0;
    idx_t i = 0;

    while (1) {
        idx_t m_next = d * a_prev - m;
        idx_t d_next = (D - m_next * m_next) / d;
        idx_t a_val  = (a0 + m_next) / d_next;

        period[i++] = a_val;
        if (a_val == 2 * a0) {     /* period ends when a_n = 2*a0 */
            period_len = i;
            break;
        }

        a_prev = a_val;
        m = m_next;
        d = d_next;
    }

    initialized = true;
}

/*---------------------------------------*/
/* public sequence function               */
/*---------------------------------------*/
seq_t a(idx_t n)
{
    if (!initialized)
        init_period();

    if (n == 0)
        return (seq_t)a0;          /* a(0) = floor(sqrt(501)) */

    /* periodic part starts at index 1 (a1 = period[0]) */
    idx_t idx = (n - 1) % period_len;
    return (seq_t)period[idx];
}
