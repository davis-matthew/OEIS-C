/*
 * A093691  a(n) is chosen so as to maximize the number of different
 *          substrings in (a(1),a(2),...,a(n)).
 * https://oeis.org/A093691
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* SMS (substrings maximizer sequence) in base 10.
   a(n) is chosen to maximize the number of distinct substrings
   of the prefix (a(1), a(2), …, a(n)).
   This implementation uses a suffix automaton updated greedily.
   No closed‑form formula is known. */

#include <stddef.h>

#define MAX_N      20005
#define MAX_STATES (2 * MAX_N)

/* A state of the suffix automaton. */
struct State {
    int len;          /* length of longest string in this class */
    int link;         /* suffix link */
    int next[10];     /* transitions on digits 0‑9, -1 if absent */
};

/* Static data shared between calls. */
static struct State st[MAX_STATES];
static idx_t     computed_len = 0;   /* number of terms already computed */
static int       sz = 0;            /* number of states used */
static int       last = 0;          /* state of the entire string so far */
static seq_t     seq_data[MAX_N];   /* stored digits of the sequence */

/* Initialise the automaton for the empty string. */
static void init_sa(void)
{
    st[0].len = 0;
    st[0].link = -1;
    for (int i = 0; i < 10; ++i) st[0].next[i] = -1;
    sz = 1;
    last = 0;
}

/* Compute a(n). The function is thread‑unsafe due to static state,
   but it meets the call‑order requirements of the OEIS evaluation. */
seq_t a(idx_t n)
{
    if (computed_len == 0) init_sa();

    /* Extend the sequence until we have the n‑th term. */
    while (computed_len < n) {
        /* Current length after appending a new digit. */
        int curLen = st[last].len + 1;

        /* Evaluate all possible next digits. */
        int bestDelta = -1;
        int bestd = 0;
        for (int d = 0; d < 10; ++d) {
            int p = last;
            while (p != -1 && st[p].next[d] == -1) p = st[p].link;

            int linkLen;
            if (p == -1) {
                linkLen = 0;
            } else {
                int q = st[p].next[d];
                linkLen = (st[p].len + 1 == st[q].len) ? st[q].len
                                                       : (st[p].len + 1);
            }
            int delta = curLen - linkLen;
            if (delta > bestDelta || (delta == bestDelta && d < bestd)) {
                bestDelta = delta;
                bestd = d;
            }
        }

        /* Now actually add the chosen digit to the automaton. */
        int cur = sz++;
        st[cur].len = st[last].len + 1;
        for (int i = 0; i < 10; ++i) st[cur].next[i] = -1;

        int p = last;
        while (p != -1 && st[p].next[bestd] == -1) {
            st[p].next[bestd] = cur;
            p = st[p].link;
        }

        if (p == -1) {
            st[cur].link = 0;
        } else {
            int q = st[p].next[bestd];
            if (st[p].len + 1 == st[q].len) {
                st[cur].link = q;
            } else {
                int clone = sz++;
                st[clone] = st[q];
                st[clone].len = st[p].len + 1;
                for (int i = 0; i < 10; ++i) if (st[clone].next[i] == q)
                    st[clone].next[i] = clone;
                while (p != -1 && st[p].next[bestd] == q) {
                    st[p].next[bestd] = clone;
                    p = st[p].link;
                }
                st[q].link = st[cur].link = clone;
            }
        }
        last = cur;

        /* Store the chosen digit. */
        seq_data[(size_t)computed_len] = (seq_t)bestd;
        ++computed_len;
    }

    return seq_data[(size_t)n - 1];
}
