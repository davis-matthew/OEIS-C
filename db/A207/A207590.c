/*
 * A207590  Number of 3 X n 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 0 1 and 1 1 1 vertically.
 * https://oeis.org/A207590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 24, u64 n <= 51, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = a(n-1) + 3*a(n-2)  (n>4)
 * with initial values a(1)=6, a(2)=36, a(3)=60, a(4)=144
 *
 * The function is memoised and works for any order of calls with n≥1.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#endif

/* Forward declaration of function */
seq_t a(idx_t n);

/* ------------------------------------------------------------------ */
/* Helper for C++ builds: uses std::vector to preserve values          */
#ifdef __cplusplus
static std::vector<seq_t> seq_vec; /* holds values 0..N (index 0 unused) */

static void ensure_size_cpp(idx_t n) {
    if (n < seq_vec.size()) return;                /* already computed */

    /* Reserve space, then compute missing terms */
    idx_t old_size = seq_vec.size();
    seq_vec.resize(n + 1);                         /* default‑construct new slots */

    idx_t start = old_size;
    if (start < 1) start = 1;                      /* first real term */

    for (idx_t i = start; i <= n; ++i) {
        if (i == 1)            seq_vec[i] = (seq_t)6;
        else if (i == 2)       seq_vec[i] = (seq_t)36;
        else if (i == 3)       seq_vec[i] = (seq_t)60;
        else if (i == 4)       seq_vec[i] = (seq_t)144;
        else                   seq_vec[i] = seq_vec[i-1] + (seq_t)3 * seq_vec[i-2];
    }
}
#endif /* __cplusplus */

/* ------------------------------------------------------------------ */
/* Helper for C builds: uses malloc/realloc which preserves data      */
static seq_t *seq = NULL;
static idx_t seq_cap = 0; /* largest index allocated (array size – 1) */
static idx_t seq_len = 0; /* largest index computed */

static void ensure_size_c(idx_t n) {
    if (n <= seq_len) return;            /* already computed */

    if (n + 1 > seq_cap) {               /* need to grow */
        idx_t new_cap = seq_cap ? seq_cap * 2 : 4;
        while (new_cap < n + 1) new_cap <<= 1;
        seq = (seq_t *)realloc(seq, sizeof(seq_t) * (new_cap + 1));
        seq_cap = new_cap;
    }

    for (idx_t i = seq_len + 1; i <= n; ++i) {
        if (i == 1)      seq[i] = (seq_t)6;
        else if (i == 2) seq[i] = (seq_t)36;
        else if (i == 3) seq[i] = (seq_t)60;
        else if (i == 4) seq[i] = (seq_t)144;
        else            seq[i] = seq[i-1] + (seq_t)3 * seq[i-2];
    }
    seq_len = n;
}

/* ------------------------------------------------------------------ */
/* Public sequence function                                           */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* offset is 1 */

#ifdef __cplusplus
    ensure_size_cpp(n);
    return seq_vec[n];
#else
    ensure_size_c(n);
    return seq[n];
#endif
}
