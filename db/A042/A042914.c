/*
 * A042914  Numerators of continued fraction convergents to sqrt(989).
 * https://oeis.org/A042914
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 21, u64 n <= 38, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the numerators of the continued‑fraction convergents
   to sqrt(989).  a(0)=31, a(1)=63, a(2)=283, …  The function follows
   the standard recurrence
          p_{-2}=0, p_{-1}=1
          p_k   = a_k*p_{k-1}+p_{k-2}
   where a_k are the coefficients of the continued fraction of sqrt(989). */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit integers.
   Returned type is seq_t for consistency with the rest of the code. */
static inline seq_t int_sqrt_u64(idx_t n)
{
    idx_t low = 0, high = (idx_t)1 << 32;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid;
        }
    }
    return (seq_t)low;
}

/* --- Continued‑fraction coefficients for sqrt(989) --------------------------------- */
static seq_t *coeff = NULL;      /* coeff[0] = a0, coeff[1..period] = periodic part */
static size_t period = 0;        /* length of the periodic part */

/* Lazy initialization of the coeff array */
static void init_coeff(void)
{
    if (coeff != NULL)
        return;   /* already initialized */

    /* sqrt(989) */
    const idx_t D = 989ULL;
    idx_t a0 = (idx_t)int_sqrt_u64(D);   /* should be 31 */

    /* Allocate space for a0 + period coefficients.
       We'll grow the array as needed. */
    size_t cap = 16;
    coeff = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!coeff)
        return;           /* memory failure – caller will crash (acceptable) */

    size_t idx = 0;
    coeff[idx++] = (seq_t)a0;    /* coeff[0] */

    /* Classic continued‑fraction expansion for sqrt(D) */
    idx_t m = 0;
    idx_t d = 1;
    idx_t a = a0;

    while (true) {
        /* Compute next triple (m, d, a) */
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;

        /* Resize if necessary */
        if (idx >= cap) {
            cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(coeff, cap * sizeof(seq_t));
            if (!tmp) {
                free(coeff);
                coeff = NULL;
                period = 0;
                return;
            }
            coeff = tmp;
        }

        coeff[idx++] = (seq_t)a;

        /* Period ends when a == 2*a0 for quadratic surds */
        if (a == (idx_t)(2 * a0))
            break;
    }

    period = idx - 1;   /* exclude a0 from period */
}

/* Helper: coefficient at index n (0‑based) */
static inline seq_t coeff_at(idx_t n)
{
    if (n == 0)
        return coeff[0];
    return coeff[1 + (size_t)((n - 1) % period)];
}

/* --- Main sequence function -------------------------------------------------------- */
seq_t a(idx_t n)
{
    init_coeff();   /* ensure coefficients are ready */

    /* Edge case n == 0 */
    if (n == 0)
        return coeff[0];

    /* Recurrence: p_{-2}=0, p_{-1}=1 */
    seq_t p_nm2 = 0;
    seq_t p_nm1 = 1;
    seq_t p_n   = 0;

    for (idx_t i = 0; i <= n; ++i) {
        seq_t a_i = coeff_at(i);
        p_n = a_i * p_nm1 + p_nm2;  /* recurrence */
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    return p_n;
}
