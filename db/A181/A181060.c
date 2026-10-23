/*
 * A181060  a(n) is the smallest positive multiple of n whose decimal digits
 *          are all 0, 1 or 2.
 * https://oeis.org/A181060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = smallest positive multiple of n whose decimal digits are
 *            all 0, 1 or 2
 * Range:     u8 n <= 8, u16 n <= 44, u32 n <= 494, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The algorithm performs a BFS on the directed graph whose
    vertices are remainders modulo n.  Each edge appends a digit 0,1 or 2
    to the current number.  The first time we reach remainder 0 we have
    found the desired multiple; the BFS guarantees minimal length and
    lexicographic order among numbers of the same length.  */

#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    seq_t   val;   /* current number (value in base 10) */
    idx_t   rem;   /* current remainder modulo n */
} Node;

seq_t a(idx_t n)
{
    if (n == 0)               /* undefined but return 0 for safety */
        return (seq_t)0;

    /* visited remainder table */
    bool *visited = (bool*)calloc((size_t)n, sizeof(bool));
    if (!visited)             /* allocation failure */
        return (seq_t)0;

    /* allocate queue for BFS, maximum size n (one state per remainder) */
    Node *queue = (Node*)malloc((size_t)n * sizeof(Node));
    if (!queue) {
        free(visited);
        return (seq_t)0;
    }

    idx_t head = 0, tail = 0;

    /* initial states: numbers 1 and 2 (no leading zero) */
    int first_digits[2] = {1, 2};
    for (int i = 0; i < 2; ++i) {
        int d = first_digits[i];
        idx_t r = (idx_t)d % n;
        seq_t v = (seq_t)d;
        if (r == 0) {
            free(queue);
            free(visited);
            return v;
        }
        if (!visited[r]) {
            visited[r] = true;
            queue[tail++] = (Node){v, r};
        }
    }

    /* BFS: iterate until we find remainder 0 */
    while (head < tail) {
        Node cur = queue[head++];
        /* allow digits 0,1,2 for subsequent positions */
        for (int d = 0; d <= 2; ++d) {
            seq_t next_val = cur.val * (seq_t)10 + (seq_t)d;
            idx_t next_rem = (cur.rem * 10 + (idx_t)d) % n;
            if (next_rem == 0) {
                free(queue);
                free(visited);
                return next_val;
            }
            if (!visited[next_rem]) {
                visited[next_rem] = true;
                if (tail < n)          /* safety check, should always hold */
                    queue[tail++] = (Node){next_val, next_rem};
            }
        }
    }

    /* In theory we should never reach this point.  For safety return 0. */
    free(queue);
    free(visited);
    return (seq_t)0;
}
