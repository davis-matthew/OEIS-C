/*
 * A189921  Wythoff representation of natural numbers.
 * https://oeis.org/A189921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf, base
 * Formula:   a(n) = Wythoff representation bit of n The sequence consists of
 *            the concatenation of the binary Wythoff representations of
 *            1,2,3,… (A for 1, B for 0). a(n) is the n‑th bit of that
 *            infinite string
 * Range:     u8 n <= 9415, u16 n <= 9415, u32 n <= 9415, u64 n <= 9415,
 *            u128 n <= 9415, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* ---------  static data  --------- */
static unsigned char *bits = NULL;   /* flat array of 0/1 bits          */
static size_t bits_len = 0;          /* number of bits stored           */
static size_t bits_cap = 0;          /* capacity of bits array          */

static int *vtype = NULL;            /* 0=unused, >0=A-index, <0=B-index */
static size_t vcap = 0;              /* capacity of vtype array         */

/* next candidate integer that is not yet marked in vtype          */
static size_t next_candidate = 1;
/* number of Wythoff pairs generated so far (A_k,B_k)             */
static size_t a_len = 0;

/* ---------  helpers  --------- */
static void ensure_bits_cap(size_t need)
{
    if (bits_cap >= need) return;
    size_t new_cap = bits_cap ? bits_cap << 1 : 64;
    while (new_cap < need) new_cap <<= 1;
    unsigned char *tmp = (unsigned char *)realloc(bits, new_cap);
    bits = tmp;
    bits_cap = new_cap;
}

static void ensure_vcap(size_t need)
{
    if (vcap >= need) return;
    size_t new_cap = vcap ? vcap << 1 : 1024;
    while (new_cap <= need) new_cap <<= 1;
    int *tmp = (int *)realloc(vtype, new_cap * sizeof(int));
    memset(tmp + vcap, 0, (new_cap - vcap) * sizeof(int));
    vtype = tmp;
    vcap = new_cap;
}

static void generate_next_pair(void)
{
    size_t k = a_len + 1;                /* new pair index              */

    /* find next unused integer for a_k */
    while (next_candidate < vcap && vtype[next_candidate] != 0)
        ++next_candidate;
    if (next_candidate >= vcap) {
        ensure_vcap(next_candidate + 1);
    }
    int a = (int)next_candidate;
    vtype[a] = (int)k;                   /* mark as member of A         */

    int b = a + (int)k;                  /* b_k = a_k + k               */
    if ((size_t)b >= vcap) {
        ensure_vcap(b + 1);
    }
    vtype[b] = -(int)k;                  /* mark as member of B         */

    ++a_len;                             /* one more pair generated     */
}

static void push_bit(unsigned char bit)
{
    if (bits_len == bits_cap) {
        ensure_bits_cap(bits_cap ? bits_cap << 1 : 64);
    }
    bits[bits_len++] = bit;
}

/* ---------  sequence function  --------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;         /* zero index never requested   */

    /* static state: next row to generate                     */
    static size_t next_row = 1;

    /* generate bits until we have at least n bits            */
    while (bits_len < (size_t)n) {

        /* make sure we have generated all pairs for this row */
        while (a_len < next_row) generate_next_pair();

        /* produce representation for row 'next_row'          */
        idx_t cur = next_row;
        if (cur == 1) {
            push_bit(1);                         /* first row is single 1      */
        } else {
            while (cur > 1) {
                int vt = vtype[(size_t)cur];
                if (vt > 0) {                    /* cur in A                  */
                    push_bit(1);
                    cur = (idx_t)vt;
                } else {                        /* cur in B                  */
                    push_bit(0);
                    cur = (idx_t)(-vt);
                }
            }
        }
        ++next_row;
    }

    return (seq_t)bits[(size_t)n - 1];
}
