/*
 * A069527  Smallest multiple of n with digit sum = 8, or 0 if no such number
 *          exists, e.g. a(3k)= 0.
 * https://oeis.org/A069527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 195, u32 n <= 898, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Smallest multiple of n whose decimal digit sum is 8.
   Returns 0 if no such multiple exists (e.g. n multiple of 3, 37, 271 or 4649). */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* safety for index 0 */

    /* Number of possible states: remainder 0..n-1 and digit sum 0..8. */
    size_t stateCount = (size_t)n * 9ULL;
    if (stateCount == 0) return (seq_t)0;      /* n == 0 handled above */

    /* State arrays */
    char      *visited = (char *)calloc(stateCount, sizeof(char));
    unsigned char *digit = (unsigned char *)calloc(stateCount, sizeof(unsigned char));
    size_t    *parent = (size_t *)calloc(stateCount, sizeof(size_t));
    if (!visited || !digit || !parent) {
        free(visited); free(digit); free(parent);
        return (seq_t)0;
    }
    /* Invalidate parent links */
    for (size_t i = 0; i < stateCount; ++i) parent[i] = stateCount;

    /* Queue for BFS */
    size_t *queue = (size_t *)malloc(stateCount * sizeof(size_t));
    if (!queue) {
        free(visited); free(digit); free(parent);
        return (seq_t)0;
    }
    size_t qhead = 0, qtail = 0;

    /* Helper: convert (remainder,sum) to a single state index */
    #define STATE_IDX(rem, sum) (((size_t)(rem) * 9ULL) + (size_t)(sum))

    /* Initial one‑digit states: numbers 1..8 (sum <= 8) */
    for (int d = 1; d <= 8; ++d) {
        unsigned long long rem = (unsigned long long)d % n;
        size_t idx = STATE_IDX(rem, d);
        if (!visited[idx]) {
            visited[idx] = 1;
            parent[idx]  = stateCount;       /* root state */
            digit[idx]   = (unsigned char)d;
            queue[qtail++] = idx;
        }
    }

    size_t found = (size_t)-1;

    /* Breadth‑first search */
    while (qhead < qtail) {
        size_t cur = queue[qhead++];
        unsigned long long curRem = cur / 9ULL;
        int curSum = (int)(cur % 9ULL);

        if (curRem == 0 && curSum == 8) {
            found = cur;
            break;
        }

        /* Try appending every digit d = 0..9 */
        for (int d = 0; d <= 9; ++d) {
            int newSum = curSum + d;
            if (newSum > 8) continue;                 /* digit sum too large */
            unsigned long long newRem = (curRem * 10ULL + d) % n;
            size_t nidx = STATE_IDX(newRem, newSum);
            if (!visited[nidx]) {
                visited[nidx] = 1;
                parent[nidx]  = cur;
                digit[nidx]   = (unsigned char)d;
                queue[qtail++] = nidx;
            }
        }
    }

    seq_t result = 0;

    /* Reconstruct the number if a solution was found */
    if (found != (size_t)-1) {
        /* Temporary buffer for digits (least‑significant first) */
        size_t bufCap = 16;
        char *buf = (char *)malloc(bufCap);
        if (!buf) { result = (seq_t)0; goto cleanup; }

        size_t len = 0;
        size_t st = found;
        while (st != stateCount) {    /* walk back to the root */
            unsigned char d = digit[st];
            if (len >= bufCap) {
                bufCap *= 2;
                char *tmp = (char *)realloc(buf, bufCap);
                if (!tmp) { free(buf); result = (seq_t)0; goto cleanup; }
                buf = tmp;
            }
            buf[len++] = '0' + d;
            st = parent[st];
        }

        /* Reverse digits to obtain the correct order */
        for (size_t i = 0; i < len / 2; ++i) {
            char tmp = buf[i];
            buf[i] = buf[len - 1 - i];
            buf[len - 1 - i] = tmp;
        }

        /* Convert decimal string to seq_t */
        for (size_t i = 0; i < len; ++i) {
            result = result * 10 + (seq_t)(buf[i] - '0');
        }

        free(buf);
    }

cleanup:
    free(queue);
    free(visited);
    free(digit);
    free(parent);
    return result;
}
