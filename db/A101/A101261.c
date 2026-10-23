/*
 * A101261  a(2n-1) = 1; a(2n) = a(n)th smallest positive integer not among
 *          the earlier terms of the sequence.
 * https://oeis.org/A101261
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2n-1) = 1, a(2n) = the a(n)-th smallest positive
 *            integer not among the earlier terms
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses a dynamic array of already used numbers and a
   dynamic array of sequence values.  It works in C and C++ as requested. */

#if defined(__cplusplus)
#   include <vector>
#   include <cstdint>
#else
#   include <stdlib.h>
#   include <stdint.h>
#endif

/*------------------------------------------------------------------*/
/* Storage for computed terms and for the “used” flag array        */
/*------------------------------------------------------------------*/
static idx_t lastIdx = 0;              /* highest index already stored */

#if defined(__cplusplus)
/* In C++ use a vector */
static std::vector<uint8_t> used;
static std::vector<seq_t> seq;         /* seq[0] unused, seq[i] holds a(i) */
#else
/* In C use raw arrays */
static uint8_t *used = NULL;
static idx_t used_len = 0;             /* allocated length of used[] */

static seq_t *seq = NULL;
static idx_t seq_len = 0;              /* allocated length of seq[] */
#endif

/*------------------------------------------------------------------*/
/*  Helper functions to enlarge the storage arrays                  */
/*------------------------------------------------------------------*/
static void ensure_used_at(idx_t i)
{
#if defined(__cplusplus)
    if (used.size() <= (size_t)i) used.resize(i + 1, 0);
#else
    if (i >= used_len) {
        idx_t new_len = used_len ? used_len << 1 : 32;
        while (new_len <= i) new_len <<= 1;
        uint8_t *tmp = (uint8_t*)realloc(used, new_len);
        if (!tmp) return;                         /* allocation failed */
        for (idx_t j = used_len; j < new_len; ++j) tmp[j] = 0;
        used = tmp;
        used_len = new_len;
    }
#endif
}

static void ensure_seq_at(idx_t n)
{
#if defined(__cplusplus)
    if (seq.size() <= (size_t)n) seq.resize(n + 1);
#else
    if (n >= seq_len) {
        idx_t new_len = seq_len ? seq_len << 1 : 32;
        while (new_len <= n) new_len <<= 1;
        seq_t *tmp = (seq_t*)realloc(seq, new_len * sizeof(seq_t));
        if (!tmp) return;                         /* allocation failed */
        for (idx_t j = seq_len; j < new_len; ++j) tmp[j] = 0;
        seq = tmp;
        seq_len = new_len;
    }
#endif
}

/*------------------------------------------------------------------*/
/* Main sequence function                                          */
/*------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n < 1) return 0;               /* sequence undefined for n<1 */

    /* One‑time initialisation */
    static int initialized = 0;
    if (!initialized) {
        initialized = 1;
        ensure_used_at(1);             /* 1 is used from the start */
        used[1] = 1;
    }

    /* Expand storage to cover up to n */
    ensure_seq_at(n);

    /* Compute missing indices up to n */
    for (idx_t i = lastIdx + 1; i <= n; ++i) {
        if (i & 1) {                    /* odd indices -> 1 */
            seq[i] = 1;
        } else {                         /* even indices */
            idx_t k = i >> 1;
            idx_t target = (idx_t)seq[k];   /* rank of missing number */

            idx_t missingCnt = 0;
            idx_t x = 2;
            for (;;) {
                ensure_used_at(x);
                if (!used[x]) {
                    ++missingCnt;
                    if (missingCnt == target) break;
                }
                ++x;
            }

            seq[i] = (seq_t)x;          /* store chosen value */
            used[x] = 1;                /* mark it as used */
        }
    }

    lastIdx = n;
    return seq[n];
}
