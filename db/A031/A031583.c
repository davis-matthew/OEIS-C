/*
 * A031583  Numbers k such that the continued fraction for sqrt(k) has even
 *          period and if the last term of the periodic part is deleted the
 *          central term is 85.
 * https://oeis.org/A031583
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A031583:
   numbers k such that the continued‑fraction for sqrt(k) has even period
   and, after deleting the last periodic term, the central term equals 85. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>

/* Integer square root for uint64_t (floor). */
static uint64_t isqrt_u64(uint64_t x) {
    uint64_t lo = 0, hi = x;
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1u) >> 1u;
        if (mid * mid <= x)
            lo = mid;
        else
            hi = mid - 1u;
    }
    return lo;
}

/* Dynamic storage of found terms. */
static seq_t *terms = NULL;
static size_t terms_len = 0;
static size_t terms_cap = 0;

/* Append a value to the dynamic array. */
static void append_term(seq_t val) {
    if (terms_len >= terms_cap) {
        size_t new_cap = terms_cap ? terms_cap << 1 : 16;
        seq_t *new_terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (terms) {
            for (size_t i = 0; i < terms_len; ++i)
                new_terms[i] = terms[i];
            free(terms);
        }
        terms = new_terms;
        terms_cap = new_cap;
    }
    terms[terms_len++] = val;
}

/* Test whether D satisfies the required property. */
static bool qualifies(uint64_t D) {
    uint64_t a0 = isqrt_u64(D);
    if (a0 * a0 == D)     /* square – discarded */
        return false;

    uint64_t m = 0, d = 1, a = a0;
    /* Periodic part stored in arr; length fits in 8192 for uint64_t D. */
    uint64_t arr[8192];
    size_t len = 0;

    while (1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        arr[len++] = a;
        if (a == 2 * a0)   /* end of period */
            break;
        if (len >= sizeof(arr) / sizeof(arr[0]))
            return false;  /* safety: defensive, should never happen */
    }

    if (len & 1u)        /* period length must be even */
        return false;

    size_t center_index = (len >> 1) - 1u;  /* zero‑based index of central term */
    return arr[center_index] == 85u;
}

/* Main sequence function. */
seq_t a(idx_t n) {
    if (n == 0u) /* undefined for 0, but return 0 to satisfy type */
        return (seq_t)0;

    static uint64_t last_tested = 0u;

    while (terms_len < (size_t)n) {
        ++last_tested;
        if (qualifies(last_tested))
            append_term((seq_t)last_tested);
    }
    return terms[n - 1u];
}
