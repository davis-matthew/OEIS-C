/*
 * A028198  Expansion of 1/((1-5x)(1-9x)(1-11x)(1-12x)).
 * https://oeis.org/A028198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=37,
 *            a(n)=23*a(n-1)-132*a(n-2)+((9^(n+1)-5^(n+1))/4), n>=2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper to grow a dynamic array of seq_t to at least new_cap elements */
static inline void grow_size(size_t *cap, seq_t **buf, size_t new_cap)
{
    if (*cap >= new_cap) return;
    size_t new_capacity = (*cap > 0) ? (*cap << 1) : 2;
    if (new_capacity < new_cap) new_capacity = new_cap;
    seq_t *tmp = (seq_t *)realloc(*buf, new_capacity * sizeof(seq_t));
    if (!tmp) abort();            /* terminate on allocation failure */
    *buf = tmp;
    *cap = new_capacity;
}

seq_t a(idx_t n)
{
    static seq_t *seq   = NULL;
    static size_t seq_cap = 0, seq_len = 0;   /* seq_len = last computed index */

    static seq_t *pow5 = NULL;
    static size_t pow5_cap = 0, pow5_len = 0;   /* highest exponent stored */

    static seq_t *pow9 = NULL;
    static size_t pow9_cap = 0, pow9_len = 0;   /* highest exponent stored */

    /* initial allocation of caches */
    if (!seq)
    {
        grow_size(&seq_cap, &seq, 2);
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)37;
        seq_len = 1;   /* last computed index */

        grow_size(&pow5_cap, &pow5, 2);
        pow5[0] = (seq_t)1;
        pow5[1] = (seq_t)5;
        pow5_len = 1;

        grow_size(&pow9_cap, &pow9, 2);
        pow9[0] = (seq_t)1;
        pow9[1] = (seq_t)9;
        pow9_len = 1;
    }

    /* compute up to n if needed */
    if ((idx_t)n > seq_len)
    {
        /* ensure powers up to exponent n+1 are available */
        size_t needed = (size_t)(n + 1);
        if (pow5_len < needed)
        {
            grow_size(&pow5_cap, &pow5, needed + 1);
            for (size_t e = pow5_len + 1; e <= needed; ++e)
                pow5[e] = pow5[e - 1] * (seq_t)5;
            pow5_len = needed;
        }
        if (pow9_len < needed)
        {
            grow_size(&pow9_cap, &pow9, needed + 1);
            for (size_t e = pow9_len + 1; e <= needed; ++e)
                pow9[e] = pow9[e - 1] * (seq_t)9;
            pow9_len = needed;
        }

        /* ensure seq array large enough */
        grow_size(&seq_cap, &seq, (size_t)(n + 1));

        for (idx_t i = seq_len + 1; i <= n; ++i)
        {
            seq_t part = 23 * seq[i - 1] - 132 * seq[i - 2];
            seq_t add  = (pow9[(size_t)(i + 1)] - pow5[(size_t)(i + 1)]) / (seq_t)4;
            seq[i] = part + add;
        }
        seq_len = (size_t)n;
    }

    return seq[(size_t)n];
}
