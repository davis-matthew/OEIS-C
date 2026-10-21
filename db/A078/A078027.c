/*
 * A078027  Expansion of (1 - x)/(1 - x^2 - x^3).
 * https://oeis.org/A078027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-2) + a(n-3) for n>=4; a(0) = 1, a(1) = -1, a(2) = 1,
 *            a(3) = 0
 * Range:     i8 n <= 25, i16 n <= 45, i32 n <= 84, i64 n <= 163,
 *            i128 n <= 321, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#ifdef __cplusplus
#include <vector>
#else
#include <stdlib.h>
#endif

seq_t a(idx_t n)
{
#ifdef __cplusplus
    /* C++ implementation using std::vector to avoid
       memmove of objects with non‑trivial constructors. */
    static std::vector<seq_t> buf;
    if (buf.empty()) {
        buf.reserve(16);
        buf.push_back(seq_t(1));   /* a(0) */
        buf.push_back(seq_t(-1));  /* a(1) */
        buf.push_back(seq_t(1));   /* a(2) */
        buf.push_back(seq_t(0));   /* a(3) */
    }

    if (n < buf.size())
        return buf[(size_t)n];

    /* Grow capacity if necessary. */
    if ((size_t)(n + 1) > buf.capacity())
        buf.reserve(std::max(buf.capacity() * 2, (size_t)n + 2));

    /* Compute missing terms. */
    for (size_t i = buf.size(); i <= (size_t)n; ++i)
        buf.push_back(buf[i - 2] + buf[i - 3]);

    return buf[(size_t)n];
#else
    /* C implementation using malloc/realloc. */
    static seq_t *buf = NULL;
    static idx_t cap = 0;   /* allocated capacity */
    static idx_t len = 0;   /* number of terms actually stored */

    if (buf == NULL) {
        /* Initial allocation and base values. */
        cap = 16;
        buf = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!buf) abort();            /* allocation failed */
        buf[0] = (seq_t)1;            /* a(0) */
        buf[1] = (seq_t)-1;           /* a(1) */
        buf[2] = (seq_t)1;            /* a(2) */
        buf[3] = (seq_t)0;            /* a(3) */
        len = 4;
    }

    if (n < len)
        return buf[(size_t)n];

    /* Grow buffer if needed. */
    if ((idx_t)(n + 1) > cap) {
        idx_t new_cap = cap;
        while (new_cap < (idx_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        buf = tmp;
        cap = new_cap;
    }

    /* Compute missing terms. */
    for (idx_t i = len; i <= n; ++i)
        buf[i] = buf[i - 2] + buf[i - 3];
    len = n + 1;

    return buf[(size_t)n];
#endif
}
