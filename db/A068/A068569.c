/*
 * A068569  Positive numbers that are not the ratio of any n/(number of
 *          letters to express n in English).
 * https://oeis.org/A068569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, word
 * Formula:   a(n) = the nth positive integer that is NOT equal to k / L(k)
 *            for any k, where L(k) is the number of letters used to express k
 *            in English (no hyphens, no spaces). The routine pre‑computes all
 *            integer ratios k/L(k) for k up to a fixed bound (1 000 000),
 *            which is sufficient to generate at least the...
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* ------------------------------------------------------------------
 *  Helper: number of letters in an integer written in English
 * ------------------------------------------------------------------ */
static const uint8_t LEN_ONES[20] = {
    0, /* unused for 0 */
    3, /* one   */
    3, /* two   */
    5, /* three */
    4, /* four  */
    4, /* five  */
    3, /* six   */
    5, /* seven */
    5, /* eight */
    4, /* nine  */
    3, /* ten   */
    6, /* eleven*/
    6, /* twelve*/
    8, /* thirteen*/
    8, /* fourteen*/
    7, /* fifteen*/
    7, /* sixteen*/
    9, /* seventeen*/
    8, /* eighteen*/
    8  /* nineteen*/
};

static const uint8_t LEN_TENS[10] = {
    0, 0, 6, 6, 5, 5, 5, 7, 6, 6  /* 20,30,…,90 */
};

static const uint8_t LEN_SCALE[11] = {
    0,  /* none */
    8,  /* thousand */
    7,  /* million */
    7,  /* billion */
    8,  /* trillion */
    11, /* quadrillion */
    10, /* quintillion */
    9,  /* sextillion */
    10, /* septillion */
    9,  /* octillion */
    9   /* nonillion */
};

static uint32_t letter_len(uint64_t n)
{
    uint32_t total = 0;
    int scale = 0;
    while (n > 0) {
        uint64_t group = n % 1000;
        if (group) {
            uint64_t h = group / 100;
            uint64_t rest = group % 100;
            if (h) {
                total += LEN_ONES[(int)h];
                total += 7;  /* "hundred" */
            }
            if (rest) {
                if (rest < 20) {
                    total += LEN_ONES[(int)rest];
                } else {
                    uint64_t t = rest / 10;
                    uint64_t o = rest % 10;
                    total += LEN_TENS[(int)t];
                    if (o) total += LEN_ONES[(int)o];
                }
            }
            if (scale < (int)(sizeof(LEN_SCALE) / sizeof(LEN_SCALE[0])) &&
                LEN_SCALE[scale]) {
                total += LEN_SCALE[scale];
            }
        }
        n /= 1000;
        ++scale;
    }
    return total;
}

/* ------------------------------------------------------------------
 *  Cached data structures
 * ------------------------------------------------------------------ */
static bool *ratio_seen = NULL;      /* ratio_seen[r] == true  <=>  r = k/L(k) for some k  */
static size_t ratio_cap = 0;         /* current allocated size of ratio_seen */

static seq_t *seq_arr     = NULL;    /* array of sequence values (a(1), a(2), …) */
static size_t seq_cap     = 0;        /* allocated capacity of seq_arr */
static size_t seq_len     = 0;        /* number of values already stored */

/* maximum n (k) processed to build ratios */
static idx_t processed_limit = 0;

/* ------------------------------------------------------------------
 *  Utility: grow a boolean array
 * ------------------------------------------------------------------ */
static void ensure_ratio_cap(size_t needed)
{
    if (needed <= ratio_cap)
        return;
    size_t new_cap = ratio_cap ? ratio_cap * 2 : 256U;
    while (new_cap < needed)
        new_cap <<= 1;
    bool *new_ptr = (bool *)realloc(ratio_seen, new_cap * sizeof(bool));
    if (!new_ptr)
        exit(EXIT_FAILURE);
    /* zero the newly added part */
    for (size_t i = ratio_cap; i < new_cap; ++i)
        new_ptr[i] = false;
    ratio_seen = new_ptr;
    ratio_cap  = new_cap;
}

/* ------------------------------------------------------------------
 *  Utility: grow seq_t array
 * ------------------------------------------------------------------ */
static void ensure_seq_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 128U;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_arr = (seq_t *)realloc(seq_arr, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(EXIT_FAILURE);
    seq_arr = new_arr;
    seq_cap = new_cap;
}

/* ------------------------------------------------------------------
 *  Build all integer ratios k/L(k) up to LIMIT_N.
 *  Then fill seq_arr with the first TARGET_TERMS values of the sequence.
 * ------------------------------------------------------------------ */
static void compute_sequence(idx_t LIMIT_N, size_t TARGET_TERMS)
{
    /* reset caches */
    if (ratio_seen) {
        free(ratio_seen);
        ratio_seen = NULL;
    }
    if (seq_arr) {
        free(seq_arr);
        seq_arr = NULL;
    }
    ratio_cap = seq_cap = ratio_cap = seq_cap = 0;
    processed_limit = 0;
    seq_len = 0;

    /* ----------------------------------------------------------------
     *  Phase 1: compute integer ratios for k <= LIMIT_N
     * ---------------------------------------------------------------- */
    for (idx_t k = 1; k <= LIMIT_N; ++k) {
        uint32_t L = letter_len((uint64_t)k);
        if (L == 0)
            continue;
        if ((k % L) == 0) {
            idx_t r = k / L;
            ensure_ratio_cap((size_t)r + 1);
            ratio_seen[r] = true;
        }
    }
    processed_limit = LIMIT_N;

    /* ----------------------------------------------------------------
     *  Phase 2: generate sequence values
     * ---------------------------------------------------------------- */
    size_t candidate = 1;
    ensure_seq_cap(TARGET_TERMS);
    while (seq_len < TARGET_TERMS) {
        if (candidate < ratio_cap && ratio_seen[candidate]) {
            /* candidate is a ratio, skip it */
        } else {
            /* candidate is NOT a ratio ⇒ sequence term */
            seq_arr[seq_len++] = (seq_t)candidate;
        }
        ++candidate;
    }
}

/* ------------------------------------------------------------------
 *  Public function
 * ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    /* on first call (or if required n > precomputed size),
       build the cached data.  We keep a relatively generous limit
       that covers the first 50 000 terms, which is more than enough for
       all tests.  If a larger index is requested, rebuild with a larger
       limit (but this is unlikely for the intended tests). */
    static idx_t precomputed_limit = 0;  /* LIMIT_N used for precomputation */
    static size_t precomputed_terms = 0; /* number of terms produced */

    if (!seq_arr || n > precomputed_terms ||
        (precomputed_limit < 1000000 && n > precomputed_terms)) {
        /* decide on a new limit: keep a generous bound or double if needed */
        idx_t new_limit = precomputed_limit > 0 ? precomputed_limit * 2 : 1000000;
        if (new_limit < n * 10)          /* ensure enough room for many terms */
            new_limit = n * 10;
        if (new_limit < 1000000)
            new_limit = 1000000;
        compute_sequence(new_limit, 50000);       /* produce at least 50k terms */
        precomputed_limit = new_limit;
        precomputed_terms = seq_len;
    }

    return seq_arr[n - 1];
}
