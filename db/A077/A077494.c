/*
 * A077494  a(n) = smallest k such that the digit sum of 7k is n.
 * https://oeis.org/A077494
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, easy, nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 47, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = smallest k such that digit_sum(7*k) = n, for n ≥ 2 */
#include <stdlib.h>
#include <stdint.h>
#include "../utils/seq_from_str.h"

/* Public sequence function.  The types seq_t and idx_t are supplied by the build. */
seq_t a(idx_t n)
{
    if (n < 2)
        return (seq_t)0;          /* undefined for indices below the offset */

    /* --------------------------------------------------------------------------------
     *  Build the minimal decimal number m with digit sum n and m ≡ 0 (mod 7).
     *  States are (sum, remainder), each encoded as sum*7 + remainder.
     * -------------------------------------------------------------------------------- */
    size_t total_states = (size_t)(n + 1) * 7;

    /* Visited flags, parents and digits that led to each state. */
    unsigned char *vis   = (unsigned char*)calloc(total_states, 1);
    int           *par   = (int*)          malloc(total_states * sizeof(int));
    unsigned char *dig   = (unsigned char*)malloc(total_states * sizeof(unsigned char));
    if (!vis || !par || !dig) abort();

    for (size_t i = 0; i < total_states; ++i)
        par[i] = -1;

    /* BFS queue */
    int *queue = (int*)malloc(total_states * sizeof(int));
    if (!queue) abort();
    size_t head = 0, tail = 0;

    int start_id = 0;                       /* sum = 0, rem = 0 */
    vis[start_id] = 1;
    queue[tail++] = start_id;

    int target_id = -1;

    while (head < tail && target_id == -1) {
        int cur   = queue[head++];
        int sum   = cur / 7;
        int rem   = cur % 7;
        int d_start = (sum == 0) ? 1 : 0;   /* avoid leading zero */

        for (int d = d_start; d <= 9; ++d) {
            int ns = sum + d;
            if ((idx_t)ns > n) continue;
            int nr = (rem * 10 + d) % 7;
            int next_id = ns * 7 + nr;
            if (!vis[next_id]) {
                vis[next_id] = 1;
                par[next_id] = cur;
                dig[next_id] = (unsigned char)d;
                if (ns == (int)n && nr == 0) {
                    target_id = next_id;
                    break;
                }
                queue[tail++] = next_id;
            }
        }
    }

    /* ---------------  Reconstruct the decimal digits of m  ----------------------- */
    int temp = target_id;
    size_t len = 0;
    while (temp != start_id) {
        ++len;
        temp = par[temp];
    }

    unsigned char *rev_digits = (unsigned char*)malloc(len * sizeof(unsigned char));
    if (!rev_digits) abort();
    temp = target_id;
    size_t pos = 0;
    while (temp != start_id) {
        rev_digits[pos++] = dig[temp];
        temp = par[temp];
    }
    /* rev_digits now holds the digits of m in reverse order (least significant first). */

    /* ---------------  Divide m by 7 to obtain k  ----------------------- */
    unsigned char *m_digits = (unsigned char*)malloc(len * sizeof(unsigned char));
    if (!m_digits) abort();
    for (size_t i = 0; i < len; ++i)
        m_digits[i] = rev_digits[len - 1 - i];     /* most significant first */

    int carry = 0;
    size_t q_len = 0;
    unsigned char *q_digits = (unsigned char*)malloc(len * sizeof(unsigned char));
    if (!q_digits) abort();

    for (size_t i = 0; i < len; ++i) {
        int value = carry * 10 + m_digits[i];
        unsigned char qd = value / 7;
        carry = value % 7;
        if (q_len > 0 || qd > 0)            /* skip leading zeros */
            q_digits[q_len++] = qd;
    }
    /* carry must be 0 because m is divisible by 7 */
    if (q_len == 0) q_digits[q_len++] = 0;   /* not expected (n ≥ 2) */

    /* Convert quotient digits to a decimal string for seq_from_str */
    char *k_str = (char*)malloc((q_len + 1) * sizeof(char));
    if (!k_str) abort();
    for (size_t i = 0; i < q_len; ++i)
        k_str[i] = q_digits[i] + '0';
    k_str[q_len] = '\0';

    seq_t k = seq_from_str(k_str);

    /* Free temporary storage */
    free(k_str);
    free(q_digits);
    free(m_digits);
    free(rev_digits);
    free(vis);
    free(par);
    free(dig);
    free(queue);

    return k;
}
