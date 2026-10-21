/*
 * A202849  Number of secondary structures of size n having no stacks of even
 *          length.
 * https://oeis.org/A202849
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of secondary structures of size n having no stacks
 *            of even length. Recurrence (valid for n >= 10): (n+2)*a(n)
 *            +(-2*n-1)*a(n-1) +(n-1)*a(n-2) +3*(-2*n+5)*a(n-3) +(-n+7)*a(n-6)
 *            +3*(2*n-17)*a(n-7) +(-n+10)*a(n-8) +(-2*n+23)*a(n-9)
 *            +(n-13)*a(n-10)=0 Initial values: a(0)=1, a(1)=1, a(2)=...
 * Range:     u8 n <= 9, u16 n <= 13, u32 n <= 25, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes for generic integer types and memory allocation */
#include <stdlib.h>
#include <stdint.h>

/* External function following the required signature */
seq_t a(idx_t n) {
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of computed values (cache[0..len-1]) */

    /* Initialize cache with base values */
    if (len == 0) {
        /* Initial capacity large enough to hold at least 16 elements */
        cap = 16;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);

        /* Base values a(0) … a(9) */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache[4] = (seq_t)4;
        cache[5] = (seq_t)7;
        cache[6] = (seq_t)14;
        cache[7] = (seq_t)31;
        cache[8] = (seq_t)66;
        cache[9] = (seq_t)141;
        len = 10;
    }

    /* If requested value already computed, return it */
    if (n < len) return cache[n];

    /* Grow cache if necessary */
    size_t needed = (size_t)n + 1;
    if (cap < needed) {
        size_t newcap = cap ? cap << 1 : 16;
        if (newcap < needed) newcap = needed;
        seq_t *ptr = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!ptr) exit(1);
        cache = ptr;
        cap = newcap;
    }

    /* Compute all values up to n */
    for (idx_t k = len; k <= n; ++k) {
        if (k < 10) { /* base values already set */
            /* nothing to do */
        } else {
            /* Compute a(k) using the integer recurrence */
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;

            /* Helper macro to add term with proper sign */
#define ADD_TERM(VAL, IDX, IDXTAG)                                  \
    do {                                                            \
        if ((VAL) == 0) break;                                     \
        if ((VAL) > 0) {                                           \
            seq_t prod = (seq_t)(((__int128)(VAL)) * ( (__int128)cache[(IDX)])); \
            sum_pos += prod;                                       \
        } else { /* negative sign */                               \
            seq_t prod = (seq_t)(((__int128)(-(VAL))) * ( (__int128)cache[(IDX)])); \
            sum_neg += prod;                                       \
        }                                                           \
    } while (0)

            /* Terms of the recurrence (coefficients may be negative) */
            ADD_TERM( (int64_t)( 2*k + 1),         k-1, 1);  /* +(2n+1) */
            ADD_TERM( (int64_t)(-(k-1)),          k-2, 2);  /* -(n-1) */
            ADD_TERM( (int64_t)( 3*(2*k-5)),      k-3, 3);  /* +3(2n-5) */
            ADD_TERM( (int64_t)( k-7),             k-6, 4);  /* +(n-7) */
            ADD_TERM( (int64_t)(-3*(2*k-17)),     k-7, 5);  /* -3(2n-17) */
            ADD_TERM( (int64_t)( k-10),           k-8, 6);  /* +(n-10) */
            ADD_TERM( (int64_t)( 2*k-23),         k-9, 7);  /* +(2n-23) */
            ADD_TERM( (int64_t)(-(k-13)),         k-10,8);  /* -(n-13) */

#undef ADD_TERM

            /* N = sum_pos - sum_neg (safe because sum_pos >= sum_neg) */
            seq_t numerator = sum_pos;
            numerator -= sum_neg;
            seq_t denom = (seq_t)(k + 2);
            cache[(size_t)k] = numerator / denom;
        }
    }
    len = n + 1;
    return cache[n];
}
