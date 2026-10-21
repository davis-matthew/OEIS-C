/*
 * A085354  a(n) = 3*4^n - (n+4)*2^(n-1).
 * https://oeis.org/A085354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*4^n - (n+4)*2^(n-1) Recurrence: a(0) = 1 a(n) =
 *            2*a(n-1) + 3*2^(2n-1) - 2^(n-1) for n >= 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *val = NULL;
    static seq_t *power2 = NULL;      /* store 2^k for current k (last computed) */
    static size_t cached = 0;         /* number of values stored in val   */
    static size_t cap    = 0;         /* capacity of allocated arrays     */
    static seq_t pow2_last = 0;       /* 2^cached-1    */  /* holds 2^(cached-1) */

    /* initialize cache on first call */
    if (cached == 0) {
        /* allocate initial space */
        const size_t init_cap = 8;
        val = (seq_t*)malloc(init_cap * sizeof(seq_t));
        power2 = (seq_t*)malloc(init_cap * sizeof(seq_t));
        if (!val || !power2) { /* allocation failed; terminate */
            abort(); /* cannot handle memory failure within this function */
        }
        /* n == 0 case */
        val[0] = (seq_t)1;
        power2[0] = (seq_t)1;            /* 2^0 */
        cached = 1;
        cap = init_cap;
        pow2_last = power2[0];           /* 2^(0) */
    }

    if (n < cached)
        return val[n];

    /* grow cache to at least n+1 */
    while (cached <= n) {
        /* ensure capacity */
        if (cached >= cap) {
            size_t new_cap = cap * 2;
            if (new_cap == 0) new_cap = 8;
            seq_t *new_val = (seq_t*)realloc(val, new_cap * sizeof(seq_t));
            seq_t *new_pow = (seq_t*)realloc(power2, new_cap * sizeof(seq_t));
            if (!new_val || !new_pow) {
                abort(); /* memory allocation failure */
            }
            val = new_val;
            power2 = new_pow;
            cap = new_cap;
        }

        /* compute next power of two: 2^cached */
        seq_t pow2_curr = pow2_last + pow2_last;   /* multiply by 2 */
        power2[cached] = pow2_curr;

        /* recurrence components */
        seq_t a_prev = val[cached - 1];
        seq_t pow2_prev = pow2_last;              /* 2^(cached-1) */

        seq_t prod = pow2_prev * pow2_curr;        /* 2^( (cached-1) + cached ) = 2^(2*cached-1) */
        seq_t term = prod * (seq_t)3;              /* 3 * 2^(2*cached-1) */

        seq_t a_curr = a_prev * (seq_t)2;         /* 2*a_prev */
        a_curr = a_curr + term - pow2_prev;       /* a_curr = 2*a_prev + 3*2^(2*cached-1) - 2^(cached-1) */

        val[cached] = a_curr;
        pow2_last = pow2_curr;                    /* update last power */
        cached++;
    }

    return val[n];
}
