/*
 * A069536  Smallest multiple of 8 with digit sum n.
 * https://oeis.org/A069536
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest multiple of 8 whose digit sum equals n.
 *            Computation by BFS over (sum, remainder modulo 8). This
 *            guarantees minimal length and lexicographically smallest number
 * Range:     u8 n <= 0, u16 n <= 38, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* a(0) = 0 */

    /* maximum possible sum */
    unsigned int max_sum = (unsigned int)n;

    /* total number of states */
    unsigned int num_states = (max_sum + 1) * 8U;

    /* visited flags, predecessor id, and digit appended */
    unsigned char *visited = (unsigned char*)calloc(num_states, 1);
    int          *prev    = (int*)calloc(num_states, sizeof(int));
    unsigned char *digit  = (unsigned char*)calloc(num_states, 1);

    /* simple queue */
    int *queue = (int*)malloc(num_states * sizeof(int));
    unsigned int head = 0, tail = 0;

    /* helper macro to compute state id */
#define ID(sum, mod) (((sum) << 3) + (mod))

    /* enqueue starting digits 1..9 */
    for (int d = 1; d <= 9; ++d) {
        if ((unsigned int)d > max_sum) continue;
        unsigned int sum = (unsigned int)d;
        unsigned int mod = d % 8;
        int id = ID(sum, mod);
        if (!visited[id]) {
            visited[id] = 1;
            prev[id] = -1;           /* start state */
            digit[id] = (unsigned char)d;
            queue[tail++] = id;
        }
    }

    /* BFS */
    int target_id = -1;
    while (head < tail) {
        int id = queue[head++];
        unsigned int sum = id >> 3;      /* id / 8 */
        unsigned int mod = id & 7U;     /* id % 8 */

        if (sum == max_sum && mod == 0U) {
            target_id = id;
            break;
        }

        for (int d = 0; d <= 9; ++d) {
            unsigned int new_sum = sum + (unsigned int)d;
            if (new_sum > max_sum) continue;
            unsigned int new_mod = (mod * 10 + d) % 8U;
            int new_id = ID(new_sum, new_mod);
            if (!visited[new_id]) {
                visited[new_id] = 1;
                prev[new_id] = id;
                digit[new_id] = (unsigned char)d;
                queue[tail++] = new_id;
            }
        }
    }

    /* reconstruct the number */
    seq_t result = 0;
    int id = target_id;
    if (id == -1) {        /* safety: should never happen */
        free(visited);
        free(prev);
        free(digit);
        free(queue);
        return 0;
    }
    /* collect digits in reverse order */
    unsigned char *rev_buf = (unsigned char*)malloc((max_sum + 1) * sizeof(unsigned char));
    unsigned int rev_len = 0;
    while (id != -1) {
        rev_buf[rev_len++] = digit[id];
        id = prev[id];
    }
    /* reverse to obtain correct order */
    for (unsigned int i = 0; i < rev_len; ++i) {
        unsigned char d = rev_buf[rev_len - 1 - i];
        result = result * 10 + d;
    }

    /* clean up */
    free(visited);
    free(prev);
    free(digit);
    free(queue);
    free(rev_buf);

    return result;
}
