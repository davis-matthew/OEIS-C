/*
 * A103700  Add 9 to each of the preceding digits, beginning with 0.
 * https://oeis.org/A103700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Breadth‑first traversal of the tree
 *  root 0 → child 9
 *  for every node x>0, let its decimal digits be d_k…d_0
 *  each digit d_i produces a child value d_i+9
 *  The sequence a(n) lists the nodes in this order, with a(0)=0.
 *  Implementation handles both C (fixed‑size integer seq_t) and
 *  C++ (arbitrary‑precision seq_t) builds using conditional
 *  compilation. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    size_t cap = 64;
    seq_t *queue;
#ifdef __cplusplus
    queue = new seq_t[cap];
#else
    queue = (seq_t *)malloc(cap * sizeof(seq_t));
#endif
    if (!queue) return (seq_t)0;

    size_t head = 0, tail = 1;
    queue[0] = (seq_t)0;

    idx_t idx = 0;
    while (head < tail) {
        seq_t cur = queue[head++];
        if (idx == n) {
#ifdef __cplusplus
            delete[] queue;
#else
            free(queue);
#endif
            return cur;
        }
        ++idx;

        if (cur == (seq_t)0) {
            /* root -> child 9 */
            if (tail == cap) {
                size_t newcap = cap << 1;
                seq_t *tmp;
#ifdef __cplusplus
                tmp = new seq_t[newcap];
                for (size_t i = 0; i < tail; ++i) tmp[i] = queue[i];
                delete[] queue;
                queue = tmp;
#else
                tmp = (seq_t *)realloc(queue, newcap * sizeof(seq_t));
                if (!tmp) { free(queue); return (seq_t)0; }
                queue = tmp;
#endif
                cap = newcap;
            }
            queue[tail++] = (seq_t)9;
            continue;
        }

        /* produce children from digits of cur */
        seq_t tmp = cur;
        seq_t digits[32];
        size_t dcnt = 0;
        while (tmp > 0) {
            digits[dcnt++] = tmp % (seq_t)10;
            tmp /= (seq_t)10;
        }
        for (size_t i = 0; i < dcnt; ++i) {
            seq_t child = digits[dcnt - 1 - i] + (seq_t)9;
            if (tail == cap) {
                size_t newcap = cap << 1;
                seq_t *tmp2;
#ifdef __cplusplus
                tmp2 = new seq_t[newcap];
                for (size_t j = 0; j < tail; ++j) tmp2[j] = queue[j];
                delete[] queue;
                queue = tmp2;
#else
                tmp2 = (seq_t *)realloc(queue, newcap * sizeof(seq_t));
                if (!tmp2) { free(queue); return (seq_t)0; }
                queue = tmp2;
#endif
                cap = newcap;
            }
            queue[tail++] = child;
        }
    }

#ifdef __cplusplus
    delete[] queue;
#else
    free(queue);
#endif
    return (seq_t)0;
}
