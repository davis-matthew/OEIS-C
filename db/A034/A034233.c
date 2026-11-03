/*
 * A034233  Number of quaternary codes of length 2 with n words.
 * https://oeis.org/A034233
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A034233:  Number of quaternary codes of length 2 with n words
 *
 *  The code uses Burnside’s lemma.  The group G is the product of
 *  two copies of the symmetric group on 4 symbols (acting independently
 *  on the two coordinates) together with the coordinate‑swap.
 *  G has |G| = 24·24·2 = 1152 elements.  For a group element g the
 *  permutation of the 16 words is determined by g; let L₁,…,L_c be the
 *  lengths of its cycles.  A subset of size n is fixed by g iff it is a
 *  union of whole cycles, i.e. the cycle lengths chosen sum to n.
 *  The number of such subsets is
 *
 *          A_g(n) = [yⁿ] ∏_{i=1}^c (1 + y^{L_i})
 *
 *  The desired number of orbits is
 *
 *          a(n) = (1/|G|) · Σ_{g∈G} A_g(n)
 *
 *  Since the total number of words is 16, a(n)=0 for n>16.  The
 *  algorithm pre‑computes, for every group element, the array
 *
 *      fixed[g][k] = A_g(k)   (k = 0..16)
 *
 *  and then returns the average.  The tables are small; all
 *  intermediate values fit into uint64_t (≤ 12870), so the
 *  implementation works for any seq_t type that supports the usual
 *  integer arithmetic.
 */

#include <stdbool.h>

/* Generate the 24 permutations of {0,1,2,3}. */
static void gen_permutations(int curr[4], bool used[4], int depth,
                             int perms[24][4], int *idx)
{
    if (depth == 4) {
        for (int i = 0; i < 4; ++i) perms[*idx][i] = curr[i];
        (*idx)++;
        return;
    }
    for (int v = 0; v < 4; ++v) {
        if (!used[v]) {
            used[v] = true;
            curr[depth] = v;
            gen_permutations(curr, used, depth + 1, perms, idx);
            used[v] = false;
        }
    }
}

/* Pre‑compute for every group element the numbers of fixed subsets
 * of each size 0..16.  The results are stored in a static table. */
static void init_fixed_counts(seq_t fixed_counts[1152][17])
{
    int perms[24][4];
    bool used[4] = { false, false, false, false };
    int curr[4];
    int perm_idx = 0;
    gen_permutations(curr, used, 0, perms, &perm_idx);   /* 24 permutations */

    int g = 0;          /* index of current group element */
    for (int a = 0; a < 24; ++a) {
        for (int b = 0; b < 24; ++b) {
            for (int swap = 0; swap < 2; ++swap) {
                int mapping[16];
                /* build the action on the 16 words */
                for (int w = 0; w < 16; ++w) {
                    int x = w / 4;
                    int y = w % 4;
                    int nx, ny;
                    if (swap) {
                        nx = perms[a][y];
                        ny = perms[b][x];
                    } else {
                        nx = perms[a][x];
                        ny = perms[b][y];
                    }
                    mapping[w] = nx * 4 + ny;
                }

                /* Find the cycle lengths of this permutation */
                bool visited[16] = {false};
                int cycle_len[16];
                int cycle_cnt = 0;

                for (int i = 0; i < 16; ++i) {
                    if (!visited[i]) {
                        int sz = 0;
                        int v = i;
                        do {
                            visited[v] = true;
                            v = mapping[v];
                            sz++;
                        } while (v != i);
                        cycle_len[cycle_cnt++] = sz;
                    }
                }

                /* Dynamic programming: dp[j] = #subsets of size j fixed by g */
                seq_t dp[17];
                for (int j = 0; j <= 16; ++j) dp[j] = 0;
                dp[0] = 1;
                for (int idx = 0; idx < cycle_cnt; ++idx) {
                    int l = cycle_len[idx];
                    for (int j = 16; j >= l; --j) {
                        dp[j] = dp[j] + dp[j - l];
                    }
                }
                for (int j = 0; j <= 16; ++j) {
                    fixed_counts[g][j] = dp[j];
                }
                ++g;
            }
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n > 16) return 0; /* no code can contain more than 16 distinct words */

    static bool init = false;
    static seq_t fixed_counts[1152][17];

    if (!init) {
        init_fixed_counts(fixed_counts);
        init = true;
    }

    /* Sum over all group elements and divide by |G| = 1152 */
    seq_t sum = 0;
    int idx = (int)n;          /* safe: n <= 16 */
    for (int g = 0; g < 1152; ++g) {
        sum = sum + fixed_counts[g][idx];
    }
    return sum / (seq_t)1152;
}
