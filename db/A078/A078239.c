/*
 * A078239  a(n) = smallest multiple of n using only prime digits, with a(n) =
 *          0 if there are none.
 * https://oeis.org/A078239
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = the smallest multiple of n whose decimal digits are all
 *            prime digits {2,3,5,7}. If no such multiple exists a(n)=0. The
 *            algorithm performs a breadth‑first search over remainders modulo
 *            n, building the minimal number while avoiding repeated
 *            remainders
 * Range:     u8 n <= 30, u16 n <= 181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* idx_t is uint64_t, seq_t is provided by the build */
#include <stdlib.h>
#include <stdbool.h>

/* Breadth‑first search over residue classes modulo n to find the
   shortest decimal string consisting only of the digits 2,3,5,7 that
   represents a multiple of n. */
seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n==0 return 0. */
    if (n == 0) return (seq_t)0;

    /* Allowable prime digits. */
    static const uint8_t allowed[4] = { 2, 3, 5, 7 };

    /* Allocate data structures of size n.  Use explicit casts to make
       the code compile under C++ as well. */
    size_t N = (size_t)n;                      /* n fits into size_t on the build */
    uint8_t *seen   = (uint8_t*)calloc(N, sizeof(uint8_t));   /* 0 = not visited */
    idx_t   *prev   = (idx_t*)malloc(N * sizeof(idx_t));     /* predecessor residue */
    uint8_t *digit  = (uint8_t*)malloc(N * sizeof(uint8_t));  /* digit that led to residue */
    idx_t   *queue  = (idx_t*)malloc(N * sizeof(idx_t));     /* BFS queue */

    if (!seen || !prev || !digit || !queue) {
        free(seen); free(prev); free(digit); free(queue);
        return (seq_t)0;              /* allocation failure - give up */
    }

    /* BFS queue indices. */
    size_t head = 0, tail = 0;

    /* Initial layer: single‑digit numbers. */
    for (int i = 0; i < 4; ++i) {
        uint8_t d = allowed[i];
        idx_t r = d % n;
        if (r == 0) {                                 /* direct multiple */
            free(seen); free(prev); free(digit); free(queue);
            return (seq_t)d;
        }
        if (!seen[r]) {
            seen[r]   = 1;
            prev[r]   = n;              /* sentinel marking start of path */
            digit[r]  = d;
            queue[tail++] = r;
        }
    }

    /* Main BFS loop. */
    while (head < tail) {
        idx_t cur = queue[head++];
        for (int i = 0; i < 4; ++i) {
            uint8_t d = allowed[i];
            /* Compute next remainder: (cur * 10 + d) mod n. */
            idx_t new_r = ( (unsigned __int128)cur * 10 + d ) % n;
            if (seen[new_r]) continue;

            seen[new_r] = 1;
            prev[new_r] = cur;
            digit[new_r] = d;

            if (new_r == 0) {                        /* found a solution */
                /* Reconstruct the number from the digit chain. */
                size_t cap = 16;
                size_t used = 0;
                uint8_t *stack = (uint8_t*)malloc(cap);
                if (!stack) {
                    free(seen); free(prev); free(digit); free(queue);
                    return (seq_t)0;
                }
                idx_t r = 0;
                while (r != n) {                     /* backtrack to start */
                    if (used == cap) {
                        cap <<= 1;
                        uint8_t *tmp = (uint8_t*)realloc(stack, cap);
                        if (!tmp) {
                            free(stack);
                            free(seen); free(prev); free(digit); free(queue);
                            return (seq_t)0;
                        }
                        stack = tmp;
                    }
                    stack[used++] = digit[r];
                    r = prev[r];
                }

                /* Build decimal value from most significant digit. */
                seq_t value = (seq_t)0;
                for (size_t i = used; i > 0; --i)
                    value = value * 10 + stack[i - 1];

                free(stack); free(seen); free(prev); free(digit); free(queue);
                return value;
            }

            queue[tail++] = new_r;
        }
    }

    /* No admissible multiple found. */
    free(seen); free(prev); free(digit); free(queue);
    return (seq_t)0;
}
