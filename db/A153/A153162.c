/*
 * A153162  Denominators of Stern-Brocot tree hanging between 1/3 and 2/3;
 *          numerators=A153161.
 * https://oeis.org/A153162
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  frac, nonn, tabf
 * Formula:   generate the denominators of the Stern‑Brocot tree hanging
 *            between 1/3 and 2/3 by breadth‑first traversal. a(1) = 2,
 *            thereafter each node p/q (with bounds a/b < p/q < c/d) produces
 *            children (a+p)/(b+q) and (p+c)/(q+d)
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* static state */
static seq_t *terms      = NULL;   /* array of sequence terms */
static size_t terms_len  = 0;       /* number of terms actually computed */
static size_t terms_cap  = 0;       /* capacity of terms array */

struct Node {
    seq_t lnum, lden;   /* left bound a/b */
    seq_t rnum, rden;   /* right bound c/d */
    seq_t num, den;     /* the fraction p/q of the current node */
};
typedef struct Node Node;

static Node *queue   = NULL;   /* queue of pending nodes */
static size_t q_head = 0;      /* index of next element to pop   */
static size_t q_tail = 0;      /* index of next free slot        */
static size_t q_cap  = 0;      /* capacity of queue array        */
static int    init   = 0;      /* root enqueued?                 */

/* enlarge terms array to at least min_cap elements */
static void grow_terms(size_t min_cap)
{
    if (terms_cap >= min_cap) return;
    size_t new_cap = terms_cap ? terms_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    seq_t *new_terms = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_terms) abort();
    if (terms_len) memcpy(new_terms, terms, terms_len * sizeof(seq_t));
    free(terms);
    terms      = new_terms;
    terms_cap  = new_cap;
}

/* enlarge queue array to accommodate at least min_cap slots */
static void grow_queue(size_t min_cap)
{
    size_t used = q_tail - q_head;
    if (q_cap >= min_cap && q_tail < q_cap) return;   /* enough space */
    size_t new_cap = q_cap ? q_cap << 1 : 16;
    if (new_cap < min_cap) new_cap = min_cap;
    Node *new_q = (Node *)malloc(new_cap * sizeof(Node));
    if (!new_q) abort();
    if (used) memcpy(new_q, queue + q_head, used * sizeof(Node));
    free(queue);
    queue = new_q;
    q_cap = new_cap;
    q_head = 0;
    q_tail = used;
}

/* enqueue a node */
static void enqueue(const Node n)
{
    size_t used = q_tail - q_head;
    if (used + 1 > q_cap || q_tail == q_cap)   /* need space */
        grow_queue(used + 1);
    queue[q_tail++] = n;
}

/* dequeue a node; caller must ensure queue not empty */
static Node dequeue(void)
{
    return queue[q_head++];
}

/* generate sequence up to index n (1‑based) */
static void generate_up_to(idx_t n)
{
    if (n == 0) return;
    size_t target = (size_t)n;
    if (terms_len >= target) return;

    grow_terms(target);

    if (!init) {
        /* root node: 1/2 with bounds 1/3 and 2/3 */
        Node root;
        root.lnum = 1;  root.lden = 3;
        root.rnum = 2;  root.rden = 3;
        root.num  = 1;  root.den  = 2;
        enqueue(root);
        init = 1;
    }

    while (terms_len < target) {
        Node cur = dequeue();
        terms[terms_len++] = cur.den;   /* store denominator of current node */

        /* left child: mediant of left bound and cur fraction */
        Node lc;
        lc.lnum = cur.lnum;  lc.lden = cur.lden;
        lc.rnum = cur.num;   lc.rden = cur.den;
        lc.num  = lc.lnum + lc.rnum;
        lc.den  = lc.lden + lc.rden;
        enqueue(lc);

        /* right child: mediant of cur fraction and right bound */
        Node rc;
        rc.lnum = cur.num;   rc.lden = cur.den;
        rc.rnum = cur.rnum;  rc.rden = cur.rden;
        rc.num  = rc.lnum + rc.rnum;
        rc.den  = rc.lden + rc.rden;
        enqueue(rc);
    }
}

/* public API */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* out‑of‑range, should not happen */
    generate_up_to(n);
    return terms[n - 1];
}
