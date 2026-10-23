/*
 * A130248  Partial sums of the Lucas Inverse A130247.
 * https://oeis.org/A130248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A130247(k) where A130247(k) is the Lucas
 *            inverse: A130247(1) = 1 A130247(2) = 0 for k >= 3, A130247(k) =
 *            maximal m with Lucas(m) <= k < Lucas(m+1)
 * Range:     u8 n <= 46, u16 n <= 4403, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for partial sums */
    static seq_t *a_cache = NULL;   /* a_cache[i] = a(i) for i >= 1, a_cache[0] = 0 */
    static idx_t a_size  = 0;        /* largest n for which a_cache is filled */

    /* static cache for Lucas numbers */
    static seq_t *lucas   = NULL;    /* lucas[i] = Lucas(i) */
    static idx_t lucas_len = 0;      /* number of stored Lucas numbers */
    static idx_t cur_k     = 2;      /* current Lucas index (>= 2) for n >= 3 */

    /* initialise lucas array on first use */
    if (lucas == NULL) {
        lucas_len = 3;
        lucas = (seq_t *)malloc(lucas_len * sizeof(seq_t));
        if (!lucas) return 0; /* out of memory – unlikely in this context */
        lucas[0] = (seq_t)2;  /* L(0) */
        lucas[1] = (seq_t)1;  /* L(1) */
        lucas[2] = (seq_t)3;  /* L(2) */
        cur_k   = 2;          /* for n >= 3 */
    }

    /* extend a_cache if needed */
    if (n > a_size) {
        a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
        if (!a_cache) return 0; /* out of memory */
        if (a_size == 0) a_cache[0] = (seq_t)0; /* initialise base case */

        /* sequentially compute missing terms */
        for (idx_t m = a_size + 1; m <= n; ++m) {
            seq_t val;

            if (m == 1)                          /* A130247(1) = 1 */
                val = (seq_t)1;
            else if (m == 2)                     /* A130247(2) = 0 */
                val = (seq_t)0;
            else {
                /* ensure lucas[cur_k+1] exists */
                while (cur_k + 1 >= lucas_len) {
                    seq_t next = lucas[lucas_len - 1] + lucas[lucas_len - 2];
                    lucas = (seq_t *)realloc(lucas, (lucas_len + 1) * sizeof(seq_t));
                    if (!lucas) return 0; /* out of memory */
                    lucas[lucas_len] = next;
                    ++lucas_len;
                }

                /* increase cur_k while next Lucas number fits in m */
                while (lucas[cur_k + 1] <= m) {
                    ++cur_k;
                    /* need to extend lucas if cur_k+1 is now out of bounds */
                    if (cur_k + 1 >= lucas_len) {
                        seq_t next = lucas[lucas_len - 1] + lucas[lucas_len - 2];
                        lucas = (seq_t *)realloc(lucas, (lucas_len + 1) * sizeof(seq_t));
                        if (!lucas) return 0; /* out of memory */
                        lucas[lucas_len] = next;
                        ++lucas_len;
                    }
                }

                val = (seq_t)cur_k;
            }

            a_cache[m] = a_cache[m - 1] + val;
        }
        a_size = n;
    }

    return a_cache[n];
}
