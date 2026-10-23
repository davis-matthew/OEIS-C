/*
 * A172055  n-th number k such that 6*k-1 is composite while 6*k+1 is prime
 *          plus n-th number m such that 6*m-1 is prime while 6*m+1 is
 *          composite.
 * https://oeis.org/A172055
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A121765(n) + A121763(n)
 * Range:     u8 n <= 38, u16 n <= 6726, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Prime test provided by the utilities */
#include "../utils/is_prime.h"

/* Expand a dynamic array of seq_t values */
static void grow_array(seq_t **arr, size_t *len, size_t *cap)
{
    if (*len >= *cap)
    {
        size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
        seq_t *tmp = (seq_t *)realloc(*arr, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);
        *arr = tmp;
        *cap = new_cap;
    }
}

/* Sequence routine */
seq_t a(idx_t n)
{
    /*    k_seq    – A121765 values  (k such that 6k‑1 composite, 6k+1 prime)
          m_seq    – A121763 values  (m such that 6m‑1 prime,   6m+1 composite)   */
    static seq_t *k_seq = NULL, *m_seq = NULL;
    static size_t k_len = 0, m_len = 0;
    static size_t k_cap = 0, m_cap = 0;

    /* Candidate integer being tested */
    static seq_t candidate = 1;

    /* Generate values until we have at least n entries in both sequences */
    while (k_len < (size_t)n || m_len < (size_t)n)
    {
        /* Compute 6*candidate-1 and 6*candidate+1 using idx_t arithmetic */
        idx_t v1 = 6 * (idx_t)candidate - 1;
        idx_t v2 = 6 * (idx_t)candidate + 1;

        bool p1 = is_prime(v1);
        bool p2 = is_prime(v2);

        /* first sequence: 6k-1 composite, 6k+1 prime */
        if (!p1 && p2)
        {
            grow_array(&k_seq, &k_len, &k_cap);
            k_seq[k_len++] = candidate;
        }

        /* second sequence: 6m-1 prime, 6m+1 composite */
        if (p1 && !p2)
        {
            grow_array(&m_seq, &m_len, &m_cap);
            m_seq[m_len++] = candidate;
        }

        /* Advance to next candidate */
        candidate++;
    }

    /* Both sequences have at least n entries – return the sum */
    return k_seq[(size_t)n - 1] + m_seq[(size_t)n - 1];
}
