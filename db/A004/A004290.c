/*
 * A004290  Least positive multiple of n that when written in base 10 uses
 *          only 0's and 1's.
 * https://oeis.org/A004290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, nice
 * Formula:   breadth-first search over remainders mod n, appending digit 0
 *            before 1, finds the smallest number made of 0's and 1's that is
 *            divisible by n (a(n) = n*A079339(n))
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 35, u64 n <= 791, u128 n <= 9998,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    bool *visited = (bool *)calloc((size_t)n, sizeof(bool));
    idx_t *parent = (idx_t *)malloc((size_t)n * sizeof(idx_t));
    int *digit = (int *)malloc((size_t)n * sizeof(int));
    idx_t *queue = (idx_t *)malloc((size_t)n * sizeof(idx_t));

    for (idx_t i = 0; i < n; i++) {
        parent[i] = (idx_t)-1;
        digit[i] = -1;
    }

    idx_t front = 0, rear = 0;
    idx_t start_remainder = 1 % n;
    queue[rear++] = start_remainder;
    visited[start_remainder] = true;

    while (front < rear) {
        idx_t r = queue[front++];
        if (r == 0) break;

        idx_t r_next = (r * 10) % n;
        if (!visited[r_next]) {
            visited[r_next] = true;
            parent[r_next] = r;
            digit[r_next] = 0;
            queue[rear++] = r_next;
        }

        r_next = (r * 10 + 1) % n;
        if (!visited[r_next]) {
            visited[r_next] = true;
            parent[r_next] = r;
            digit[r_next] = 1;
            queue[rear++] = r_next;
        }
    }

    int *path = (int *)malloc((size_t)(n + 1) * sizeof(int));
    idx_t path_len = 0;

    for (idx_t r = 0; r != start_remainder; r = parent[r]) {
        path[path_len++] = digit[r];
    }
    path[path_len++] = 1;

    seq_t result = 0;
    for (idx_t i = path_len; i > 0; i--) {
        result = result * 10 + path[i - 1];
    }

    free(visited);
    free(parent);
    free(digit);
    free(queue);
    free(path);

    return result;
}
