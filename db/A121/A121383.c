/*
 * A121383  a(0)=3. a(n) = number of earlier terms which, each added to n, sum
 *          to a square.
 * https://oeis.org/A121383
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of earlier terms which, each added to n, sum to a
 *            square
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *seq    = NULL;
    static size_t seq_len = 0;  /* number of terms already computed */
    static size_t seq_cap = 0;  /* capacity of seq array              */

    static size_t *freq   = NULL;
    static size_t freq_cap = 0; /* capacity of freq array              */

    /* ensure capacity for seq array */
    if (seq_cap <= (size_t)n) {
        size_t new_cap = seq_cap ? seq_cap << 1 : 1;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;
        seq = tmp;
        seq_cap = new_cap;
    }

    /* ensure capacity for frequency array */
    if (freq_cap <= (size_t)n + 1) {
        size_t new_cap = freq_cap ? freq_cap << 1 : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        size_t *tmp = (size_t *)realloc(freq, new_cap * sizeof(size_t));
        if (!tmp) return (seq_t)0;
        /* initialise new entries to 0 */
        for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
        freq = tmp;
        freq_cap = new_cap;
    }

    /* maximum value seen so far (used for bounding r loops) */
    static size_t max_val = 0;

    /* compute terms up to n */
    for (size_t k = seq_len; k <= (size_t)n; ++k) {
        if (k == 0) {
            seq[0] = 3;
            /* initialise frequency of the first term */
            if (freq_cap <= 3) {
                size_t new_cap = freq_cap ? freq_cap << 1 : 4;
                size_t *tmp = (size_t *)realloc(freq, new_cap * sizeof(size_t));
                if (!tmp) return (seq_t)0;
                for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
                freq = tmp;
                freq_cap = new_cap;
            }
            freq[3] = 1;
            max_val = 3;
        } else {
            /* ceil(sqrt(k)) */
            seq_t s_f_k = floor_sqrt((seq_t)k);
            size_t r_start = (size_t)s_f_k;
            if ((seq_t)r_start * (seq_t)r_start < (seq_t)k) ++r_start;

            /* floor(sqrt(k + max_val)) */
            seq_t s_f_end = floor_sqrt((seq_t)(k + max_val));
            size_t r_end = (size_t)s_f_end;

            seq_t cnt = 0;
            for (size_t r = r_start; r <= r_end; ++r) {
                seq_t sq = (seq_t)r * (seq_t)r;
                seq_t v = sq - (seq_t)k;   /* v >= 0 by construction */
                if ((size_t)v < freq_cap)
                    cnt += (seq_t)freq[(size_t)v];
            }

            seq[k] = cnt;

            /* update frequency table if necessary */
            size_t idx = (size_t)cnt;
            if (idx >= freq_cap) {
                size_t new_cap = freq_cap ? freq_cap << 1 : 1;
                while (new_cap <= idx) new_cap <<= 1;
                size_t *tmp = (size_t *)realloc(freq, new_cap * sizeof(size_t));
                if (!tmp) return (seq_t)0;
                for (size_t i = freq_cap; i < new_cap; ++i) tmp[i] = 0;
                freq = tmp;
                freq_cap = new_cap;
            }
            ++freq[idx];

            if (cnt > max_val) max_val = (size_t)cnt;
        }
        ++seq_len;
    }

    return seq[n];
}
