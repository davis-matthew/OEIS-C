/*
 * A239219  Numbers n such that n^3 is not divisible by any of its nonzero
 *          digits.
 * https://oeis.org/A239219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   sequence defined by testing whether n^3 is not divisible by any
 *            of its nonzero decimal digits
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 1464, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored successively found terms */
    static size_t cached_len = 0; /* number of stored terms */
    static size_t cached_cap = 0; /* allocated capacity of cache */
    static idx_t next_candidate = 1; /* smallest integer not yet tested */

    while (cached_len < n) {
        idx_t m = next_candidate;
        /* find next valid n */
        while (1) {
            seq_t m_seq = (seq_t)m;
            seq_t cube = m_seq * m_seq * m_seq; /* m^3 */
            seq_t tmp = cube;
            bool ok = true;
            while (tmp > 0) {
                seq_t digit = tmp % 10;
                if (digit != 0 && cube % digit == 0) {
                    ok = false;
                    break;
                }
                tmp /= 10;
            }
            if (ok) {
                /* store m in cache */
                if (cached_len == cached_cap) {
                    size_t new_cap = cached_cap ? cached_cap * 2 : 4;
                    void *tmp_ptr = realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp_ptr) abort(); /* memory error */
                    cache = (seq_t *)tmp_ptr;
                    cached_cap = new_cap;
                }
                cache[cached_len++] = m_seq;
                next_candidate = m + 1;
                break; /* next number ready */
            }
            ++m; /* try next integer */
        }
    }
    return cache[n - 1];
}
