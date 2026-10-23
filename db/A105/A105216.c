/*
 * A105216  Maximum denominator among the n! ratios equal to the continued
 *          fractions which have the permutations of (1,2,3,...,n) for terms.
 * https://oeis.org/A105216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A105216: Maximum denominator among the n! ratios equal to the
 * continued fractions which have the permutations of (1,2,3,...,n) for terms.
 *
 * Formula (for n ≥ 5):
 *   a(n) = n*a(n-1) + (n-1)*a(n-3) + a(n-4)
 * Initial values:
 *   a(1) = 1
 *   a(2) = 2
 *   a(3) = 7
 *   a(4) = 31
 *
 * This implementation uses memoisation and is valid for both C11 and C++17.
 * For C++ builds it uses std::vector to correctly construct arbitrary‑precision
 * integer types.  For C builds it falls back to malloc/realloc on a raw array.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> _seq;   /* memoisation table */
static idx_t _last_computed = 0;

static void ensure_to(idx_t n)
{
    if (n <= _last_computed)
        return;

    _seq.resize(n + 1); /* indices 0 .. n (0 unused) */

    for (idx_t i = _last_computed + 1; i <= n; ++i)
    {
        if (i == 1)
            _seq[i] = (seq_t)1;
        else if (i == 2)
            _seq[i] = (seq_t)2;
        else if (i == 3)
            _seq[i] = (seq_t)7;
        else if (i == 4)
            _seq[i] = (seq_t)31;
        else
        {
            seq_t t1 = (seq_t)i * _seq[i - 1];
            seq_t t2 = (seq_t)(i - 1) * _seq[i - 3];
            _seq[i] = t1 + t2 + _seq[i - 4];
        }
    }
    _last_computed = n;
}
#else
static seq_t *seq      = NULL; /* raw array */
static idx_t capacity  = 0;
static idx_t last_computed = 0;

static void ensure_to(idx_t n)
{
    if (n <= last_computed)
        return;

    if (seq == NULL) {
        capacity = n + 1 > 8 ? n + 1 : 8;
        seq = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!seq) exit(1); /* allocation failed */
    } else if (n + 1 > capacity) {
        capacity = capacity * 2;
        if (capacity < n + 1)
            capacity = n + 1;
        seq = (seq_t *)realloc(seq, capacity * sizeof(seq_t));
        if (!seq) exit(1);
    }

    for (idx_t i = last_computed + 1; i <= n; ++i)
    {
        if (i == 1)
            seq[i] = (seq_t)1;
        else if (i == 2)
            seq[i] = (seq_t)2;
        else if (i == 3)
            seq[i] = (seq_t)7;
        else if (i == 4)
            seq[i] = (seq_t)31;
        else
        {
            seq_t t1 = (seq_t)i * seq[i - 1];
            seq_t t2 = (seq_t)(i - 1) * seq[i - 3];
            seq[i] = t1 + t2 + seq[i - 4];
        }
    }
    last_computed = n;
}
#endif

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* not part of the sequence */
    ensure_to(n);
#ifdef __cplusplus
    return _seq[n];
#else
    return seq[n];
#endif
}
