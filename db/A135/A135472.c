/*
 * A135472  Shortest and lexicographically earliest string of decimal digits
 *          with property that when made into cycle every pair of digits from
 *          0,0 to 9,9 can be seen exactly once.
 * https://oeis.org/A135472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base, nice
 * Formula:   not stated
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Lexicographically earliest Eulerian cycle in the
   de Bruijn graph of order 2 on digits 0–9.
   a(n) = nth digit of the unique 100‑digit string
          that contains each ordered pair of digits 0–9 exactly once. */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* The de Bruijn graph has 10 vertices (digits 0..9).
       Each vertex has 10 outgoing edges (to all digits including itself).
       An Eulerian cycle visits every edge once.
       Using Hierholzer's algorithm with edges processed in
       ascending order yields the lexicographically earliest
       such cycle.  The resulting 100‑digit cycle is stored
       in the static array 'cycle'.  For n > 100 we return the
       value cyclically (the sequence is periodic). */
    static bool built = false;
    static uint8_t cycle[100];   /* 0–9 digits, fits in seq_t */

    if (!built) {
        const int K = 10;                /* alphabet size */
        int next_edge[10];
        for (int i = 0; i < K; ++i) next_edge[i] = 0;

        int stack[101];
        int sp = 0;                      /* stack pointer */

        int rev[101];                    /* vertices in reverse order */
        int revlen = 0;

        stack[sp++] = 0;                /* start at vertex 0 */

        while (sp > 0) {
            int v = stack[sp - 1];
            if (next_edge[v] < K) {      /* choose smallest unused edge */
                int u = next_edge[v]++;  /* u = 0..9 */
                stack[sp++] = u;
            } else {                     /* no more outgoing edges */
                rev[revlen++] = v;
                --sp;
            }
        }

        /* rev holds the vertices in reverse.  The last vertex
           in rev is the same as rev[0].  The cycle itself
           consists of the first 100 vertices of the reversed path. */
        for (int i = 0; i < 100; ++i)
            cycle[i] = (uint8_t)rev[revlen - 1 - i];

        built = true;
    }

    if (n <= 100)
        return (seq_t)cycle[n - 1];
    else
        /* For completeness: repeat the cycle if n > 100. */
        return (seq_t)cycle[(size_t)(n - 1) % 100u];
}
