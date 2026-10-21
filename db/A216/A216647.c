/*
 * A216647  a(n) := card{cos((2^(k-1))*Pi/n): k=1,2,...}.
 * https://oeis.org/A216647
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = card{cos((2^(k-1))*Pi/n) : k=1,2,...}
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset starts at 1 */

    unsigned long long max = 2ULL * (unsigned long long)n;
    unsigned char *visited = (unsigned char*)calloc(max, 1);
    unsigned char *cos_visited = (unsigned char*)calloc(max/2 + 1, 1);

    unsigned long long r = 1 % max;          /* r = 2^(k-1) modulo max */
    unsigned long long count = 0;

    while (!visited[r]) {
        visited[r] = 1;
        unsigned long long m = r;
        if (m > max - m) m = max - m;       /* cos(rπ/n) = cos((max-r)π/n) */
        if (!cos_visited[m]) {
            cos_visited[m] = 1;
            ++count;
        }
        r = (r * 2ULL) % max;               /* next element in the orbit */
    }

    free(visited);
    free(cos_visited);
    return (seq_t)count;
}
