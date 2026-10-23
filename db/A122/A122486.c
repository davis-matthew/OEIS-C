/*
 * A122486  a(n) = Sum_{k=0..n} |Stirling1(n,k)|*Bell(k)^2.
 * https://oeis.org/A122486
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} |Stirling1(n,k)| * Bell(k)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

struct Signed {
    seq_t abs;   /* absolute value of the Stirling number */
    int   sign;  /* +1, -1, or 0 for zero */
};

static inline struct Signed signed_mul(const struct Signed *x, idx_t m)
{
    struct Signed r = *x;
    r.abs = r.abs * (seq_t)m;
    if (r.abs == 0) r.sign = 0;
    return r;
}

/* signed a - b */
static inline struct Signed signed_sub(const struct Signed *a, const struct Signed *b)
{
    struct Signed r;
    if (a->abs == 0) {
        r.abs  = b->abs;
        r.sign = -b->sign;
        return r;
    }
    if (b->abs == 0) {
        r.abs  = a->abs;
        r.sign = a->sign;
        return r;
    }
    if (a->sign == b->sign) {
        if (a->abs >= b->abs) {
            r.abs  = a->abs - b->abs;
            r.sign = a->sign;
        } else {
            r.abs  = b->abs - a->abs;
            r.sign = -a->sign;
        }
    } else {
        r.abs  = a->abs + b->abs;
        r.sign = a->sign;
    }
    if (r.abs == 0) r.sign = 0;
    return r;
}

seq_t a(idx_t n)
{
    /* ---------- static storage for memoisation ---------- */
    static size_t cap = 0;                     /* allocated capacity */
    static size_t max_n = (size_t)-1;          /* highest computed n */

    static seq_t   *a_vals = NULL;    /* a[0..cap-1]                */
    static seq_t   *bell   = NULL;    /* Bell[0..cap-1]              */
    static seq_t   *bell_sq = NULL;   /* Bell^2[0..cap-1]            */
    static struct Signed *s_prev = NULL;   /* previous row of Stirling1   */
    static struct Signed *s_curr = NULL;   /* current row of Stirling1    */

    /* enlarge storage if needed */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = (size_t)n + 1;
        a_vals   = (seq_t  *)realloc(a_vals,   new_cap * sizeof(seq_t));
        bell     = (seq_t  *)realloc(bell,     new_cap * sizeof(seq_t));
        bell_sq  = (seq_t  *)realloc(bell_sq,  new_cap * sizeof(seq_t));
        s_prev   = (struct Signed *)realloc(s_prev, new_cap * sizeof(struct Signed));
        s_curr   = (struct Signed *)realloc(s_curr, new_cap * sizeof(struct Signed));
        for (size_t i = cap; i < new_cap; ++i) {
            a_vals[i]   = 0;
            bell[i]     = 0;
            bell_sq[i]  = 0;
            s_prev[i].abs = 0; s_prev[i].sign = 0;
            s_curr[i].abs = 0; s_curr[i].sign = 0;
        }
        cap = new_cap;
    }

    /* ---------- base initialisation (once) ---------- */
    if (max_n == (size_t)-1) {
        a_vals[0]     = (seq_t)1;               /* a(0) = 1 */
        bell[0]       = (seq_t)1;               /* Bell(0) = 1 */
        bell_sq[0]    = (seq_t)1;               /* 1^2 = 1 */
        s_prev[0].abs = (seq_t)1;               /* Stirling1(0,0) = 1 */
        s_prev[0].sign = 1;
        max_n = 0;
    }

    /* ---------- compute up to requested n ---------- */
    for (size_t curr = max_n + 1; curr <= (size_t)n; ++curr) {
        /* ---- Bell numbers via Pascal recurrence ---- */
        seq_t binom = (seq_t)1;
        seq_t sum   = (seq_t)0;
        for (size_t k = 0; k < curr; ++k) {
            sum += binom * bell[k];
            if (k + 1 < curr) {
                binom = binom * (seq_t)(curr - 1 - k);
                binom = binom / (seq_t)(k + 1);
            }
        }
        bell[curr]    = sum;
        bell_sq[curr] = bell[curr] * bell[curr];

        /* ---- Stirling1(curr, k) for k=1..curr ---- */
        s_curr[0].abs = 0; s_curr[0].sign = 0;
        for (size_t k = 1; k <= curr; ++k) {
            struct Signed term1 = s_prev[k - 1];              /* s(curr-1,k-1) */
            struct Signed term2 = signed_mul(&s_prev[k], (idx_t)(curr - 1)); /* (curr-1)*s(curr-1,k) */
            s_curr[k] = signed_sub(&term1, &term2);          /* s(curr,k) = term1 - term2 */
        }

        /* ---- a(curr) ---- */
        seq_t a_curr = (seq_t)0;
        for (size_t k = 1; k <= curr; ++k) {
            a_curr += s_curr[k].abs * bell_sq[k];
        }
        a_vals[curr] = a_curr;

        /* ---- prepare for next iteration: swap rows ---- */
        struct Signed *tmp = s_prev;
        s_prev = s_curr;
        s_curr = tmp;
    }

    max_n = (size_t)n;
    return a_vals[n];
}
