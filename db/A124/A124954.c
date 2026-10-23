/*
 * A124954  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of circular n‑digit base‑26 numbers with adjacent
   digits differing by ≤2.
   The sequence equals trace(A^n) where A[i][j] = 1 if |i−j|≤2
   (0≤i,j≤25) and 0 otherwise.  a(0) is defined to be 1.  */
#include <stdbool.h>

static void mat_mul(const seq_t a[26][26], const seq_t b[26][26],
                    seq_t res[26][26]) {
    for (int i = 0; i < 26; ++i) {
        for (int j = 0; j < 26; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 26; ++k)
                sum += a[i][k] * b[k][j];
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* lazy initialization of the adjacency and identity matrices */
    static bool initialized = false;
    static seq_t adj[26][26];
    static seq_t ident[26][26];

    if (!initialized) {
        for (int i = 0; i < 26; ++i) {
            for (int j = 0; j < 26; ++j) {
                int diff = (i >= j) ? i - j : j - i;
                adj[i][j] = (diff <= 2) ? 1 : 0;
                ident[i][j] = (i == j) ? 1 : 0;
            }
        }
        initialized = true;
    }

    /* matrices for exponentiation */
    seq_t power[26][26];
    seq_t current[26][26];
    seq_t temp[26][26];

    /* power = I, current = adj */
    for (int i = 0; i < 26; ++i)
        for (int j = 0; j < 26; ++j) {
            power[i][j] = ident[i][j];
            current[i][j] = adj[i][j];
        }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(power, current, temp);
            for (int i = 0; i < 26; ++i)
                for (int j = 0; j < 26; ++j)
                    power[i][j] = temp[i][j];
        }
        mat_mul(current, current, temp);
        for (int i = 0; i < 26; ++i)
            for (int j = 0; j < 26; ++j)
                current[i][j] = temp[i][j];

        exp >>= 1;
    }

    /* trace of power matrix */
    seq_t result = 0;
    for (int i = 0; i < 26; ++i)
        result += power[i][i];

    return result;
}
