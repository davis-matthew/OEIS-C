/*
 * A041973  Denominators of continued fraction convergents to sqrt(509).
 * https://oeis.org/A041973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; for n>=1, a(n)=a_n * a(n-1)+a(n-2), where a_n are the
 *            partial quotients of the continued fraction of √509. The code
 *            builds the convergent denominators incrementally, keeping a
 *            cache so that arbitrary order calls are efficient
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 21, u64 n <= 43, u128 n <= 84,
 *            big any n
 * Helpers:   alloc_seq_array, grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/grow_seq_array.h"

typedef uint64_t idx_t;   /* idx_t is defined by the build configuration */

/* The function defined by the problem specification */
seq_t a(idx_t n)
{
    /* static cache and state for repeated calls */
    static seq_t *den = NULL;          /* cached denominators a(0..max_cached) */
    static size_t den_cap = 0;          /* current capacity of den array   */
    static size_t den_len = 0;          /* number of elements stored      */
    static int initialized = 0;        /* flag to run one‑time init      */

    /* Continued fraction state for √509 */
    static uint64_t cf_m = 0, cf_d = 1; /* m_k, d_k of the algorithm     */
    static uint64_t cf_a = 0;           /* a_k (partial quotient)         */

    /* recurrence variables for denominators */
    static seq_t q_prev1;  /* q_{k-1}  */
    static seq_t q_prev2;  /* q_{k-2}  */

    /* ----------------------------------------------------------- */
    /* One‑time initialization                                    */
    /* ----------------------------------------------------------- */
    if (!initialized) {
        const uint64_t D = 509;             /* the square‑free constant   */
        const uint64_t sqrtD = 22;          /* floor(sqrt(509))           */

        cf_a = sqrtD;                       /* a_0                          */

        /* allocate initial cache */
        den_cap = 16;
        den = alloc_seq_array(den_cap);
        den[0] = 1;                          /* a(0) = 1                     */
        den_len = 1;

        /* set recurrence state for n=0 */
        q_prev1 = den[0];   /* q_0 = 1  */
        q_prev2 = 0;        /* q_{-1} = 0 */

        initialized = 1;
    }

    /* ----------------------------------------------------------- */
    /* Handle n = 0 directly                                       */
    /* ----------------------------------------------------------- */
    if (n == 0) {
        return den[0];
    }

    /* ----------------------------------------------------------- */
    /* Extend cache up to requested index n                       */
    /* ----------------------------------------------------------- */
    while (den_len <= n) {
        /* ensure enough capacity */
        if (den_cap == 0) {
            den_cap = 16;
            den = alloc_seq_array(den_cap);
        } else if (den_len >= den_cap) {
            grow_seq_array(&den, &den_cap, den_cap * 2);
        }

        /* compute next partial quotient a_{den_len} */
        uint64_t m_next = cf_d * cf_a - cf_m;
        uint64_t d_next = (509 - m_next * m_next) / cf_d;
        uint64_t a_next = (22 + m_next) / d_next;  /* √509 has sqrtD = 22 */

        /* update continued fraction state for next iteration */
        cf_m = m_next;
        cf_d = d_next;
        cf_a = a_next;

        /* compute denominator q_{den_len} using recurrence:
           q_n = a_n * q_{n-1} + q_{n-2}                           */
        seq_t q_new = (seq_t)a_next * q_prev1 + q_prev2;

        /* store new denominator */
        den[den_len] = q_new;
        den_len++;

        /* shift recurrence variables for next step */
        q_prev2 = q_prev1;
        q_prev1 = q_new;
    }

    return den[n];
}
