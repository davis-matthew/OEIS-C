/*
 * A025630  Duplicate of A036566.
 * https://oeis.org/A025630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = the n-th smallest positive integer of the form 7^i * 8^j
 *            (i,j ≥ 0), sorted in increasing order and with duplicates
 *            removed
 * Range:     i8 n <= 6, i16 n <= 17, i32 n <= 17, i64 n <= 17, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#else
#include <stdlib.h>
#endif

/* ---- internal storage ----------------------------------------------- */

#if defined(__cplusplus)
/* C++ build: use a std::vector to manage the sequence elements. */
static std::vector<seq_t> seq;
static idx_t i7 = 0;      /* index of next multiple by 7         */
static idx_t i8 = 0;      /* index of next multiple by 8         */
#else
/* C build: raw dynamic array.  seq_t is a POD type. */
static seq_t *seq = NULL;
static idx_t len = 0;     /* number of stored terms              */
static idx_t cap = 0;     /* allocated capacity                   */
static idx_t i7 = 0;      /* index of next multiple by 7         */
static idx_t i8 = 0;      /* index of next multiple by 8         */

static void ensure_capacity(idx_t new_cap)
{
    if (new_cap <= cap)
        return;
    if (cap == 0)
        cap = 16;
    while (cap < new_cap)
        cap <<= 1;
    seq = (seq_t *)realloc(seq, cap * sizeof(seq_t));
}
#endif

/* ---- public function ----------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 0)           /* undefined for offset 1, keep harmless */
        return (seq_t)0;

#if defined(__cplusplus)
    /* First call: initialize the vector. */
    if (seq.empty())
    {
        seq.reserve(16);
        seq.push_back((seq_t)1);   /* a(1) = 1 */
        i7 = i8 = 0;
    }
    /* generate terms until we have at least n of them */
    while (seq.size() < n)
    {
        seq_t next7 = seq[i7] * (seq_t)7;
        seq_t next8 = seq[i8] * (seq_t)8;
        seq_t next;
        if (next7 < next8) {
            next = next7;
            ++i7;
        } else if (next8 < next7) {
            next = next8;
            ++i8;
        } else {                 /* equal – advance both */
            next = next7;
            ++i7; ++i8;
        }
        seq.push_back(next);
    }
    return seq[n - 1];
#else
    /* C build */
    if (!seq) {                      /* first call */
        ensure_capacity(1);
        seq[0] = (seq_t)1;           /* a(1) = 1 */
        len = 1;
        i7 = i8 = 0;
    }

    while (len < n) {
        seq_t next7 = seq[i7] * (seq_t)7;
        seq_t next8 = seq[i8] * (seq_t)8;
        seq_t next;
        if (next7 < next8) {
            next = next7;
            ++i7;
        } else if (next8 < next7) {
            next = next8;
            ++i8;
        } else {                     /* duplicate */
            next = next7;
            ++i7; ++i8;
        }
        if (len == cap)                /* grow array if needed */
            ensure_capacity(len + 1);
        seq[len++] = next;
    }
    return seq[n - 1];
#endif
}
