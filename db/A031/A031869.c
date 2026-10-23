/*
 * A031869  Lexically first 6-mark Golomb ruler.
 * https://oeis.org/A031869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: Exhaustive search for the lexicographically first optimal
 * 6‑mark Golomb ruler.
 * The ruler has marks (in 0‑based coordinates) 0,1,4,10,12,17.
 * a(n) returns the (n‑th) coordinate + 1 (OEIS offset 1). */

#include <stdbool.h>

/* Helper: recursively place marks */
static bool dfs(seq_t *coords, bool *used, int depth, int maxLen) {
    if (depth == 6)                     /* all 6 marks placed */
        return true;
    int prev = (int)coords[depth - 1];   /* previous mark (fits in int) */
    for (int pos = prev + 1; pos <= maxLen; ++pos) {
        int diffs[6];                    /* temporary differences */
        bool ok = true;
        for (int i = 0; i < depth; ++i) {
            int d = pos - (int)coords[i];
            if (used[d]) {               /* duplicate difference */
                ok = false;
                break;
            }
            diffs[i] = d;
        }
        if (!ok) continue;
        /* commit the differences */
        for (int i = 0; i < depth; ++i) used[diffs[i]] = true;
        coords[depth] = pos;
        if (dfs(coords, used, depth + 1, maxLen))
            return true;                 /* solution found */
        /* backtrack */
        for (int i = 0; i < depth; ++i) used[diffs[i]] = false;
    }
    return false;                        /* no placement found */
}

/* Compute the coordinates of the optimal 6‑mark Golomb ruler once */
static void init_golomb(seq_t *out) {
    static bool done = false;
    if (done) return;                    /* already computed */

    seq_t coords[6];
    bool used[200] = {false};            /* differences up to 199 */

    coords[0] = 0;                       /* first mark */
    coords[1] = 1;                       /* minimal second mark */
    used[1] = true;                      /* difference 1 used */

    /* Incrementally increase the maximal allowed mark */
    for (int limit = 1; limit < 200; ++limit) {
        if (dfs(coords, used, 2, limit)) { /* solution found */
            for (int i = 0; i < 6; ++i) out[i] = coords[i];
            done = true;
            break;
        }
    }
}

/* Sequence a(n) */
seq_t a(idx_t n) {
    static seq_t golomb[6] = {0};        /* 0‑based coordinates */
    static bool init = false;

    if (!init) {
        init_golomb(golomb);
        init = true;
    }

    if (n >= 1 && n <= 6)
        return golomb[n - 1] + 1;        /* OEIS offset 1 */
    return 0;                            /* undefined beyond length */
}
